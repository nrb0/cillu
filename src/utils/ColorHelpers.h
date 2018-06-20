#pragma once

#include "Illuminator.h"

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{
namespace ColorHelpers
{

void RGBtoHSV(float red, float green, float blue, float& hue, float& saturation, float& value);
void HSVtoRGB(float hue, float saturation, float value, float& red, float& green, float& blue);

Color fromHSV(float hue, float saturation, float value);
Color blend(Color first, Color second);

//----------------------------------------------------------------------------------------------------------------------

}
}
