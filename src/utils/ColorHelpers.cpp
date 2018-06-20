#include "ColorHelpers.h"

#include <algorithm>
#include <cmath>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{
namespace ColorHelpers
{

//----------------------------------------------------------------------------------------------------------------------

void computeHSV(const Color color, float& hue, float& saturation, float& value)
{
    const float red = color.red();
    const float green = color.green();
    const float blue = color.blue();

    float fCMax = std::max(std::max(red, green), blue);
    float fCMin = std::min(std::min(red, green), blue);
    float fDelta = fCMax - fCMin;

    if(fDelta > 0)
    {
        if(fCMax == red)
        {
            hue = 60 * (fmod(((green - blue) / fDelta), 6));
        }
        else if(fCMax == green)
        {
            hue = 60 * (((blue - red) / fDelta) + 2);
        }
        else if(fCMax == blue)
        {
            hue = 60 * (((red - green) / fDelta) + 4);
        }

        if(fCMax > 0)
        {
            saturation = fDelta / fCMax;
        }
        else
        {
            saturation = 0;
        }

        value = fCMax;
    }
    else
    {
        hue = 0;
        saturation = 0;
        value = fCMax;
    }

    if(hue < 0)
    {
        hue = 360 + hue;
    }

    hue = hue / 360.;
}

//----------------------------------------------------------------------------------------------------------------------

Color fromHSV(const float hue, const float saturation, const float value)
{
    Color result;
    float fC = value * saturation; // Chroma
    float fHPrime = fmod((hue * 360.) / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = value - fC;

    if(0 <= fHPrime && fHPrime < 1)
    {
        result.setRed(fC);
        result.setGreen(fX);
        result.setBlue(0);
    }
    else if(1 <= fHPrime && fHPrime < 2)
    {
        result.setRed(fX);
        result.setGreen(fC);
        result.setBlue(0);
    }
    else if(2 <= fHPrime && fHPrime < 3)
    {
        result.setRed(0);
        result.setGreen(fC);
        result.setBlue(fX);
    }
    else if(3 <= fHPrime && fHPrime < 4)
    {
        result.setRed(0);
        result.setGreen(fX);
        result.setBlue(fC);
    }
    else if(4 <= fHPrime && fHPrime < 5)
    {
        result.setRed(fX);
        result.setGreen(0);
        result.setBlue(fC);
    }
    else if(5 <= fHPrime && fHPrime < 6)
    {
        result.setRed(fC);
        result.setGreen(0);
        result.setBlue(fX);
    }
    else
    {
        result.setRed(0);
        result.setGreen(0);
        result.setBlue(0);
    }

    result.setRed(result.red() + fM);
    result.setGreen(result.green() + fM);
    result.setBlue(result.blue() + fM);

    return result;
}

//----------------------------------------------------------------------------------------------------------------------

Color blend(Color src, Color dst)
{
    auto computeColor = [&src, &dst](const float srcColor, const float dstColor)
    {
        return (srcColor * src.alpha() + dstColor * dst.alpha() * (1 - src.alpha()));
    };

    return Color(computeColor(src.red(), dst.red()),
                 computeColor(src.green(), dst.green()),
                 computeColor(src.blue(), dst.blue()),
                 src.alpha() + dst.alpha() * (1. - src.alpha()));
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace ColorHelpers
} // namespace cillu
