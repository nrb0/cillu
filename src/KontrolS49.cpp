#include "KontrolS49.h"

#include "Illuminator.h"

#include <boost/range/irange.hpp>

#include <algorithm>
#include <cmath>
#include <string>

//--------------------------------------------------------------------------------------------------

namespace cillu
{

//--------------------------------------------------------------------------------------------------

KontrolS49::KontrolS49()
: sl::cabl::Client({"KOMPLETE KONTROL S49", 0x17CC, 0x1350, sl::cabl::DeviceDescriptor::Type::HID })
, m_illuminator(std::make_unique<Illuminator>())
{
}

//--------------------------------------------------------------------------------------------------

KontrolS49::~KontrolS49() = default;

//--------------------------------------------------------------------------------------------------

void KontrolS49::onTimer()
{
    m_illuminator->update();
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

void KontrolS49::buttonChanged(sl::cabl::Device::Button button_, bool buttonState_, bool shiftState_)
{
    std::string log = "But#" + std::to_string(static_cast<int>(button_)) + " " + (buttonState_ ? " true" : " false");
    M_LOG(log);
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_)
{
    std::string log = "Enc#" + std::to_string(static_cast<int>(encoder_)) + ( valueIncreased_ ? " increased" : " decreased" );
    M_LOG(log);

}

//--------------------------------------------------------------------------------------------------

void KontrolS49::keyChanged(unsigned index_, double value_, bool shiftPressed_)
{
    device()->setKeyLed(index_, {static_cast<uint8_t>(value_ * 0xff)});
}

//--------------------------------------------------------------------------------------------------

void KontrolS49::controlChanged(unsigned pot_, double value_, bool shiftPressed_)
{
    std::string log = "Pot#" + std::to_string(static_cast<int>(pot_)) + " " + std::to_string(static_cast<int>(value_ * 100));
    M_LOG(log);
}

//--------------------------------------------------------------------------------------------------

} // namespace cillu