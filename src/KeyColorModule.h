#pragma once

#include <memory>

namespace cillu
{

class EnvelopeGenerator;

class KeyColorModule
{

public:

    KeyColorModule();
    ~KeyColorModule();

    void onTimer();

    void noteOn();
    void noteOff();

    void setHue(float);
    void setSaturation(float);
    void setBrightness(float);

    void setAttack(float);
    void setDecay(float);
    void setRelease(float);

    float getHue() const { return m_hue; }
    float getSaturation() const { return m_saturation; }
    float getBrightness() const { return m_brightness; }

private:

    float m_hue = 0;
    float m_saturation = 0;
    float m_brightness = 0;

    std::unique_ptr<EnvelopeGenerator> m_eg;

};

} // namespace cillu
