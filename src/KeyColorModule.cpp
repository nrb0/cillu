#include "KeyColorModule.h"

#include "EnvelopeGenerator.h"

#include <algorithm>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

KeyColorModule::KeyColorModule()
: m_eg(std::make_unique<EnvelopeGenerator>())
{
}

//----------------------------------------------------------------------------------------------------------------------

KeyColorModule::~KeyColorModule() = default;

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::noteOn()
{
    m_noteOn = std::min(m_noteOn +1, 2);

    m_eg->gate(m_noteOn != 0);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::noteOff()
{
    m_noteOn = std::max(m_noteOn - 1, 0);
    m_eg->gate(m_noteOn != 0);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setHue(const float hue)
{
    m_hue = hue;
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setSaturation(const float saturation)
{
    m_saturation = saturation;
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setBrightness(const float brightness)
{
    m_brightnessCoef = brightness;
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::onTimer()
{
    m_brightness = m_eg->processValue() * m_brightnessCoef;
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setAttack(const float value)
{
    m_eg->setAttackRate(value);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setDecay(const float value)
{
    m_eg->setDecayRate(value);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setRelease(const float value)
{
    m_eg->setReleaseRate(value);
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace cillu
