#pragma once

#include <thread>

namespace cillu
{

class EnvelopeGenerator
{

public:

    EnvelopeGenerator();
    ~EnvelopeGenerator();

    float processValue();

    void gate(bool);

    void setAttackRate(float rate);
    void setDecayRate(float rate);
    void setSustainLevel(float level);
    void setReleaseRate(float rate);

    void setAttackTargetRatio(float ratio);
    void setDecayReleaseTargetRatio(float ratio);

    bool isIdle() const { return m_state == State::Idle; }

private:

    enum class State
    {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };

    void updateParameters();

    void setOutput(float value);

    float m_output = 0.0;

    float m_attackRate = 0.0;
    float m_attackTargetRatio = 0.0;
    float m_attackCoef = 0.0;
    float m_attackBase = 0.0;

    float m_decayRate = 0.0;
    float m_releaseRate = 0.0;
    float m_decayReleaseTargetRatio = 0.0;
    float m_decayCoef = 0.0;
    float m_releaseCoef = 0.0;
    float m_decayBase = 0.0;
    float m_releaseBase = 0.0;

    float m_sustainLevel = 1.0;

    bool m_gate = false;

    State m_state = State::Idle;

};

} // namespace cillu
