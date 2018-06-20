#include "ColorHelpers.h"

#include <algorithm>
#include <cmath>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{
namespace ColorHelpers
{

//----------------------------------------------------------------------------------------------------------------------

void RGBtoHSV(const float red, const float green, const float blue, float& hue, float& saturation, float& value)
{
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
}

//----------------------------------------------------------------------------------------------------------------------

void fromHSV(const float hue, const float saturation, const float value, float& red, float& green, float& blue)
{
    float fC = value * saturation; // Chroma
    float fHPrime = fmod(hue / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = value - fC;

    if(0 <= fHPrime && fHPrime < 1)
    {
        red = fC;
        green = fX;
        blue = 0;
    }
    else if(1 <= fHPrime && fHPrime < 2)
    {
        red = fX;
        green = fC;
        blue = 0;
    }
    else if(2 <= fHPrime && fHPrime < 3)
    {
        red = 0;
        green = fC;
        blue = fX;
    }
    else if(3 <= fHPrime && fHPrime < 4)
    {
        red = 0;
        green = fX;
        blue = fC;
    }
    else if(4 <= fHPrime && fHPrime < 5)
    {
        red = fX;
        green = 0;
        blue = fC;
    }
    else if(5 <= fHPrime && fHPrime < 6)
    {
        red = fC;
        green = 0;
        blue = fX;
    }
    else
    {
        red = 0;
        green = 0;
        blue = 0;
    }

    red += fM;
    green += fM;
    blue += fM;
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
