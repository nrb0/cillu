#pragma once

#include <utils/Color.h>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{
namespace ColorHelpers
{

void computeHSV(Color color, float& hue, float& saturation, float& value);
Color fromHSV(float hue, float saturation, float value);

Color fromHSV(float hue, float saturation, float value);
Color blend(Color first, Color second);

//----------------------------------------------------------------------------------------------------------------------

}
}
