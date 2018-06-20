#pragma once

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

class Color
{
public:
    Color();

    Color(float red, float green, float blue);
    Color(float red, float green, float blue, float alpha);

    void setRGB(float, float, float);
    void setRGBA(float, float, float, float);

    void setRed(float red);
    void setGreen(float green);
    void setBlue(float blue);
    void setAlpha(float alpha);

    float red() const { return m_red; }
    float green() const { return m_green; }
    float blue() const { return m_blue; }
    float alpha() const { return m_alpha; }

private:
    float m_red = 0.;
    float m_green = 0.;
    float m_blue = 0.;
    float m_alpha = 1.;
};

//----------------------------------------------------------------------------------------------------------------------

}
