#include "Color.h"

#include <IlluminatorHelpers.h>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

Color::Color()
{}

//----------------------------------------------------------------------------------------------------------------------

Color::Color(unsigned red, unsigned green, unsigned blue)
{
    setRGB(red, green, blue);
}

//----------------------------------------------------------------------------------------------------------------------

Color::Color(float hue, float saturation, float value)
{
    setHSV(hue, saturation, value);
}

//----------------------------------------------------------------------------------------------------------------------

void Color::setRGB(unsigned red, unsigned green, unsigned blue)
{
    m_red = red;
    m_green = green;
    m_blue = blue;
}

//----------------------------------------------------------------------------------------------------------------------

void Color::setHSV(float hue, float saturation, float value)
{
    float r,g,b;
    IlluminatorHelpers::HSVtoRGB(hue, saturation, value, r, g, b);
    m_red = r * 256;
    m_green = g * 256;
    m_blue = b * 256;
}

//----------------------------------------------------------------------------------------------------------------------

void Color::setHue(float hue)
{
    float prevHue, saturation, value, r, g, b;
    IlluminatorHelpers::RGBtoHSV(m_red, m_green, m_blue, prevHue, saturation, value);
    IlluminatorHelpers::HSVtoRGB(hue, saturation, value, r, g, b);
    m_red = r * 256;
    m_green = g * 256;
    m_blue = b * 256;
}

//----------------------------------------------------------------------------------------------------------------------

void Color::setSaturation(float saturation)
{
    float hue, prevSaturation, value, r, g, b;
    IlluminatorHelpers::RGBtoHSV(m_red, m_green, m_blue, hue, prevSaturation, value);
    IlluminatorHelpers::HSVtoRGB(hue, saturation, value, r, g, b);
    m_red = r * 256;
    m_green = g * 256;
    m_blue = b * 256;
}

//----------------------------------------------------------------------------------------------------------------------

void Color::setBrightness(float value)
{
    float hue, saturation, prevValue, r, g, b;
    IlluminatorHelpers::RGBtoHSV(m_red, m_green, m_blue, hue, saturation, prevValue);
    IlluminatorHelpers::HSVtoRGB(hue, saturation, value, r, g, b);
    m_red = r * 256;
    m_green = g * 256;
    m_blue = b * 256;
}

//----------------------------------------------------------------------------------------------------------------------

float Color::hue() const
{
    float hue, saturation, value;
    IlluminatorHelpers::RGBtoHSV(m_red, m_green, m_blue, hue, saturation, value);
    return hue;
}

//----------------------------------------------------------------------------------------------------------------------

float Color::saturation() const
{
    float hue, saturation, value;
    IlluminatorHelpers::RGBtoHSV(m_red, m_green, m_blue, hue, saturation, value);
    return saturation;
}

//----------------------------------------------------------------------------------------------------------------------

float Color::value() const
{
    float hue, saturation, value;
    IlluminatorHelpers::RGBtoHSV(m_red, m_green, m_blue, hue, saturation, value);
    return value;
}

//----------------------------------------------------------------------------------------------------------------------

}
