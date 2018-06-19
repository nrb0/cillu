#include "KontrolS49.h"

#include "Illuminator.h"
#include "MidiMessage.h"

#include <boost/range/irange.hpp>

#include <RtMidi.h>

#include <algorithm>
#include <cmath>
#include <string>

//--------------------------------------------------------------------------------------------------

namespace
{
    constexpr unsigned MIDI_MIN = 0;
    constexpr unsigned MIDI_MAX = 127;
}

//--------------------------------------------------------------------------------------------------

namespace cillu
{

//--------------------------------------------------------------------------------------------------

KontrolS49::KontrolS49()
: sl::cabl::Client({"KOMPLETE KONTROL S49", 0x17CC, 0x1350, sl::cabl::DeviceDescriptor::Type::HID })
, m_midiOut(std::make_unique<RtMidiOut>())
, m_illuminator(std::make_unique<Illuminator>())
{
    try
    {
        m_midiOut->openVirtualPort("cillu");
    }
    catch (RtMidiError& error)
    {
        std::string strError = "[KontrolS49] RtMidiError: " + error.getMessage();
        M_LOG(strError);
    }
}

//--------------------------------------------------------------------------------------------------

KontrolS49::~KontrolS49() = default;

//--------------------------------------------------------------------------------------------------

void KontrolS49::onTimer()
{
    m_illuminator->update();

    if (!device())
    {
        return;
    }

    const size_t currentOctave = device()->currentOctave();
    for (const size_t index : boost::irange(0, 49))
    {
        Illuminator::RGB color = m_illuminator->getKeyColor(currentOctave + index, true);
        uint8_t red = static_cast<uint8_t>(color.r * 127.);
        uint8_t green = static_cast<uint8_t>(color.g * 127.);
        uint8_t blue = static_cast<uint8_t>(color.b * 127.);
        if (red == 0 && green == 0 && blue == 0)
        {
            color = m_illuminator->getKeyColor(currentOctave + index, false);
            red = static_cast<uint8_t>(color.r * 127.);
            green = static_cast<uint8_t>(color.g * 127.);
            blue = static_cast<uint8_t>(color.b * 127.);
        }

        if (index == 29)
        {
            printf("");
        }

        device()->setKeyLed(index, { red, green, blue });
    }
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::initDevice()
{
    M_LOG("Init device");

    for(const size_t index : boost::irange(0, 49))
    {
        device()->setKeyLed(index, 0);
    }
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::disconnected()
{
    M_LOG("Disconnected device");
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::sendControlChange(const unsigned channel, const unsigned id, const unsigned value)
{
    MidiMessage message;
    message.makeController(channel, id, value);
    sendMIDIMessage(message);
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::sendMIDIMessage(MidiMessage& message)
{
    if (!m_midiOut)
        return;

    m_midiOut->sendMessage(&message);
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::render()
{
    if (auto* firstDisplay = device()->textDisplay(0))
    {
        firstDisplay->putText("CHAIN:", 1);
        auto chainPair = m_mappingValue.find(CHAIN_SELECTOR);
        const unsigned value = chainPair != m_mappingValue.end() ? m_mappingValue[CHAIN_SELECTOR] : 0;
        firstDisplay->putText(std::to_string(value), 2);
    }
    for (const size_t index : boost::irange(1, 9))
    {
        if (sl::cabl::TextDisplay* display = device()->textDisplay(index))
        {
            auto mappingPair = m_mappingValue.find(SCREEN_ENCODERS + index - 1);
            const unsigned value = mappingPair != m_mappingValue.end() ? mappingPair->second : 0;
            display->putValue(value / 127., 0);
            display->putText("########", 1);
            display->putText("########", 2);
        }
    }
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::buttonChanged(sl::cabl::Device::Button button_, bool buttonState_, bool)
{
    switch(button_)
    {
    case sl::cabl::Device::Button::PresetUp:
    case sl::cabl::Device::Button::PresetDown:
        if (buttonState_)
            incDecParameter(CHAIN_SELECTOR, button_ == sl::cabl::Device::Button::PresetUp);
    default:
        break;
    }
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_)
{
    std::string log = "Enc#" + std::to_string(static_cast<int>(encoder_)) + ( valueIncreased_ ? " increased" : " decreased" );
    M_LOG(log);
    if (encoder_ > 0 && encoder_ <= 8)
    {
        incDecParameter(SCREEN_ENCODERS + encoder_ - 1, valueIncreased_);
    }
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
    std::string log = "Key#" + std::to_string(static_cast<int>(index_)) + " " + std::to_string(static_cast<int>(value_ * 100));
    //M_LOG(log);
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::controlChanged(unsigned pot_, double value_, bool shiftPressed_)
{
    std::string log = "Pot#" + std::to_string(static_cast<int>(pot_)) + " " + std::to_string(static_cast<int>(value_ * 100));
    M_LOG(log);
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::incDecParameter(const unsigned id, const bool increment)
{
    auto mappingPair = m_mappingValue.find(id);
    int value = 0;
    if (mappingPair != m_mappingValue.end())
    {
        value = std::clamp<int>(increment ? mappingPair->second + 1 : mappingPair->second - 1, MIDI_MIN, MIDI_MAX);
    }

    if (mappingPair == m_mappingValue.end() || mappingPair->second != value)
    {
        m_mappingValue[id] = static_cast<unsigned>(value);
        sendControlChange(0, id, static_cast<unsigned>(value));
        requestDeviceUpdate();
    }
}

//--------------------------------------------------------------------------------------------------

} // namespace cillu
