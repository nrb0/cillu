#include "EnvelopeGenerator.h"

#include <chrono>
#include <iostream>
#include <cmath>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

namespace
{

float calcCoef(const float rate, const float targetRatio)
{
    return (rate <= 0) ? 0.0 : exp(-log((1.0 + targetRatio) / targetRatio) / rate);
}

}

//----------------------------------------------------------------------------------------------------------------------

EnvelopeGenerator::EnvelopeGenerator()
{
    setAttackTargetRatio(0.3);
    setDecayReleaseTargetRatio(0.0001);
}

//----------------------------------------------------------------------------------------------------------------------

EnvelopeGenerator::~EnvelopeGenerator() = default;

//----------------------------------------------------------------------------------------------------------------------

float EnvelopeGenerator::processValue()
{
    switch (m_state)
    {
    case State::Idle:
        if (m_gate)
        {
            m_state = State::Attack;
        }

        break;

    case State::Attack:
        m_output = m_attackBase + m_output * m_attackCoef;

        if (m_output >= 1.0)
        {
            m_output = 1.0;
            m_state = State::Decay;
        }

        if (!m_gate)
        {
            m_state = State::Release;
        }

        break;

    case State::Decay:
        m_output = m_decayBase + m_output * m_decayCoef;

        if (m_output <= m_sustainLevel)
        {
            m_output = m_sustainLevel;
            m_state = State::Sustain;
        }

        if (!m_gate)
        {
            m_state = State::Release;
        }

        break;

    case State::Sustain:
        if (!m_gate)
        {
            m_state = State::Release;
        }

        break;

    case State::Release:
        m_output = m_releaseBase + m_output * m_releaseCoef;

        if (m_output <= 0.0)
        {
            m_output = 0.0;
            m_state = State::Idle;
        }

        if (m_gate)
        {
            m_state = State::Attack;
        }

        break;
    }

    return m_output;
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::updateParameters()
{
    m_attackCoef = calcCoef(m_attackRate, m_attackTargetRatio);
    m_decayCoef = calcCoef(m_decayRate, m_decayReleaseTargetRatio);
    m_releaseCoef = calcCoef(m_releaseRate, m_decayReleaseTargetRatio);

    m_attackBase = (1.0 + m_attackTargetRatio) * (1.0 - m_attackCoef);
    m_decayBase = (m_sustainLevel - m_decayReleaseTargetRatio) * (1.0 - m_decayCoef);
    m_releaseBase = - m_decayReleaseTargetRatio * (1.0 - m_releaseCoef);
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::setAttackRate(const float rate)
{
    m_attackRate = rate;
    updateParameters();
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::setDecayRate(const float rate)
{
    m_decayRate = rate;
    updateParameters();
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::setSustainLevel(const float level)
{
    m_sustainLevel = level;
    updateParameters();
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::setReleaseRate(const float rate)
{
    m_releaseRate = rate;
    updateParameters();
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::setAttackTargetRatio(const float targetRatio)
{
    m_attackTargetRatio = targetRatio < 0.000000001 ? 0.000000001 : targetRatio;
    updateParameters();
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::setDecayReleaseTargetRatio(const float targetRatio)
{
    m_decayReleaseTargetRatio = targetRatio < 0.000000001 ? 0.000000001: targetRatio;
    updateParameters();
}

//----------------------------------------------------------------------------------------------------------------------

void EnvelopeGenerator::gate(const bool enabled)
{
    m_gate = enabled;
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace cillu
