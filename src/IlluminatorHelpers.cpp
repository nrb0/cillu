#include "IlluminatorHelpers.h"

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{
namespace IlluminatorHelpers
{

//----------------------------------------------------------------------------------------------------------------------

//void RGBtoHSV(const float red, const float green, const float blue, float& hue, float& saturation, float& value)
//{
//    float fCMax = std::max(std::max(red, green), blue);
//    float fCMin = std::min(std::min(red, green), blue);
//    float fDelta = fCMax - fCMin;

//    if(fDelta > 0)
//    {
//        if(fCMax == red)
//        {
//            hue = 60 * (fmod(((green - blue) / fDelta), 6));
//        }
//        else if(fCMax == green)
//        {
//            hue = 60 * (((blue - red) / fDelta) + 2);
//        }
//        else if(fCMax == blue)
//        {
//            hue = 60 * (((red - green) / fDelta) + 4);
//        }

//        if(fCMax > 0)
//        {
//            saturation = fDelta / fCMax;
//        }
//        else
//        {
//            saturation = 0;
//        }

//        value = fCMax;
//    }
//    else
//    {
//        hue = 0;
//        saturation = 0;
//        value = fCMax;
//    }

//    if(hue < 0)
//    {
//        hue = 360 + hue;
//    }
//}

////----------------------------------------------------------------------------------------------------------------------

//void HSVtoRGB(const float hue, const float saturation, const float value, float& red, float& green, float& blue)
//{
//    float fC = value * saturation; // Chroma
//    float fHPrime = fmod(hue / 60.0, 6);
//    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
//    float fM = value - fC;

//    if(0 <= fHPrime && fHPrime < 1)
//    {
//        red = fC;
//        green = fX;
//        blue = 0;
//    }
//    else if(1 <= fHPrime && fHPrime < 2)
//    {
//        red = fX;
//        green = fC;
//        blue = 0;
//    }
//    else if(2 <= fHPrime && fHPrime < 3)
//    {
//        red = 0;
//        green = fC;
//        blue = fX;
//    }
//    else if(3 <= fHPrime && fHPrime < 4)
//    {
//        red = 0;
//        green = fX;
//        blue = fC;
//    }
//    else if(4 <= fHPrime && fHPrime < 5)
//    {
//        red = fX;
//        green = 0;
//        blue = fC;
//    }
//    else if(5 <= fHPrime && fHPrime < 6)
//    {
//        red = fC;
//        green = 0;
//        blue = fX;
//    }
//    else
//    {
//        red = 0;
//        green = 0;
//        blue = 0;
//    }

//    red += fM;
//    green += fM;
//    blue += fM;
//}

////----------------------------------------------------------------------------------------------------------------------

//void resetKeys(Illuminator::HSVs& keys)
//{
//    assignColor(keys, { 0, 0, 0 });
//}

////----------------------------------------------------------------------------------------------------------------------

//void decreaseBrightness(Illuminator::HSVs& keys, const float step)
//{
//    for (Illuminator::HSV& color : keys)
//    {
//        color = getDarker(color, step);
//    }
//}

////----------------------------------------------------------------------------------------------------------------------

//void increaseBrightness(Illuminator::HSVs& keys, const float step)
//{
//    for (Illuminator::HSV& color : keys)
//    {
//        color = getBrighter(color, step);
//    }
//}

////----------------------------------------------------------------------------------------------------------------------

//void assignColor(Illuminator::HSVs& keys, Illuminator::HSV newColor)
//{
//    for (Illuminator::HSV& color : keys)
//    {
//        color = newColor;
//    }
//}

////----------------------------------------------------------------------------------------------------------------------

//void assignColor(Illuminator::HSVs& keys, const float hue, const float saturation, const float value)
//{
//    for (Illuminator::HSV& hsv : keys)
//    {
//        hsv.h = hue;
//        hsv.s = saturation;
//        hsv.v = value;
//    }
//}

////----------------------------------------------------------------------------------------------------------------------

//void assignColor(Illuminator::HSVs& keys, const size_t index, Illuminator::HSV newColor)
//{
//    if (index < keys.size())
//    {
//        keys[index] = newColor;
//    }
//}

////----------------------------------------------------------------------------------------------------------------------

//void assignColor(Illuminator::HSVs& keys, const size_t index, const float hue, const float saturation, const float value)
//{
//    if (index < keys.size())
//    {
//        keys[index].h = hue;
//        keys[index].s = saturation;
//        keys[index].v = value;
//    }
//}

////----------------------------------------------------------------------------------------------------------------------

//Illuminator::HSV getBrighter(const Illuminator::HSV& color, const float step)
//{
//    Illuminator::HSV newColor = color;
//    float hue = 0;
//    float saturation = 0;
//    float value = 0;

//    RGBtoHSV(newColor.h, newColor.s, newColor.v, hue, saturation, value);
//    //HSVtoRGB(hue, saturation, std::min(value + step, 1.f), newColor.h, newColor.s, newColor.v);

//    return newColor;
//}

////----------------------------------------------------------------------------------------------------------------------

//Illuminator::HSV getDarker(const Illuminator::HSV& color, const float step)
//{
//    Illuminator::HSV newColor = color;
//    float hue = 0;
//    float saturation = 0;
//    float value = 0;

//    RGBtoHSV(newColor.h, newColor.s, newColor.v, hue, saturation, value);
//    //HSVtoRGB(hue, saturation, std::max(value - step, 0.f), newColor.h, newColor.s, newColor.v);

//    return newColor;
//}

//----------------------------------------------------------------------------------------------------------------------

} // namespace IlluminatorHelpers
} // namespace cillu
