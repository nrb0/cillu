#pragma once

#include "utils/Color.h"
#include "EnvelopeGenerator.h"

#include <memory>

namespace cillu
{

class KeyColorModule
{

public:
    void update();

    void gate(bool);

    void setAttack(float);
    void setDecay(float);
    void setRelease(float);

    void setRed(float);
    void setGreen(float);
    void setBlue(float);

    const Color& getColor() { return m_color; }

private:
    int m_gate = 0;

    Color m_color;
    EnvelopeGenerator m_eg;

};

} // namespace cillu
