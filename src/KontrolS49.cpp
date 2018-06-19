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
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
    std::string log = "Key#" + std::to_string(static_cast<int>(index_)) + " " + std::to_string(static_cast<int>(value_ * 100));
    M_LOG(log);
}

//--------------------------------------------------------------------------------------------------

} // namespace cillu
