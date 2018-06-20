#include "KeyColorModule.h"

#include "EnvelopeGenerator.h"

#include <algorithm>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::update()
{
    m_color.setAlpha(m_eg.processValue());
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::gate(const bool enabled)
{
    if (enabled)
    {
        m_gate = std::min(m_gate + 1, 2);
        m_eg.gate(m_gate != 0);
    }
    else
    {
        m_gate = std::max(m_gate - 1, 0);
        m_eg.gate(m_gate != 0);
    }
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setAttack(const float value)
{
    m_eg.setAttackRate(value * 200);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setDecay(const float value)
{
    m_eg.setDecayRate(value * 200);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setRelease(const float value)
{
    m_eg.setReleaseRate(value * 200);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setRed(const float red)
{
    m_color.setRed(red);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setGreen(const float green)
{
    m_color.setGreen(green);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setBlue(const float blue)
{
    m_color.setBlue(blue);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyColorModule::setColor(const Color& color)
{
    m_color.setRGB(color.red(), color.green(), color.blue());
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace cillu
