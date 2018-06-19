#pragma once

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

class Color
{
public:
    Color();

    Color(unsigned red, unsigned green, unsigned blue);
    Color(float hue, float saturation, float value);

    void setRGB(unsigned, unsigned, unsigned);
    void setHSV(float, float, float);

    void setRed(unsigned red) { m_red = red; }
    void setGreen(unsigned green) { m_green = green; }
    void setBlue(unsigned blue) { m_blue = blue; }

    void setHue(float);
    void setSaturation(float);
    void setBrightness(float);

    void setAlpha(unsigned alpha) { m_alpha = alpha; }

    unsigned red() const { return m_red; }
    unsigned green() const { return m_green; }
    unsigned blue() const { return m_blue; }

    float hue() const;
    float saturation() const;
    float value() const;

    unsigned alpha() const { return m_alpha; }

private:
    unsigned m_red = 0;
    unsigned m_green = 0;
    unsigned m_blue = 0;

    unsigned m_alpha = 255;
};

//----------------------------------------------------------------------------------------------------------------------

}
