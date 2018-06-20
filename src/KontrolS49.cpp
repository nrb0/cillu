#include "KontrolS49.h"

#include "MidiMessage.h"

#include <utils/ColorHelpers.h>

#include <boost/range/irange.hpp>

#include <RtMidi.h>

#include <algorithm>
#include <functional>
#include <string>

//--------------------------------------------------------------------------------------------------

namespace cillu
{

//--------------------------------------------------------------------------------------------------

KontrolS49::KontrolS49()
: sl::cabl::Client({"KOMPLETE KONTROL S49", 0x17CC, 0x1350, sl::cabl::DeviceDescriptor::Type::HID })
, m_background("cilluBack")
, m_foreground("cilluFront")
{
}

//--------------------------------------------------------------------------------------------------

KontrolS49::~KontrolS49() = default;

//--------------------------------------------------------------------------------------------------

void KontrolS49::onTimer()
{
    m_background.update();
    m_foreground.update();

    if (!device())
    {
        return;
    }

    const size_t currentOctave = device()->currentOctave();
    for (const size_t index : boost::irange(0, 49))
    {
        Color foreground = m_foreground.getColor(currentOctave + index);
        Color background = m_background.getColor(currentOctave + index);
        Color blend = ColorHelpers::blend(ColorHelpers::blend(foreground, background), Color());


        auto red = static_cast<uint8_t>(blend.red() * 127.);
        auto green = static_cast<uint8_t>(blend.green() * 127.);
        auto blue = static_cast<uint8_t>(blend.blue() * 127.);

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

void KontrolS49::render()
{
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::keyChanged(unsigned index_, double value_, bool)
{
    std::string log = "Key#" + std::to_string(static_cast<int>(index_)) + " " + std::to_string(static_cast<int>(value_ * 100));
    M_LOG(log);
}

//--------------------------------------------------------------------------------------------------

} // namespace cillu
