#pragma once

#include "Illuminator.h"

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{
namespace IlluminatorHelpers
{

//----------------------------------------------------------------------------------------------------------------------

Illuminator::HSV getBrighter(const Illuminator::HSV& color, float step = 0.05);
Illuminator::HSV getDarker(const Illuminator::HSV& color, float step = 0.05);

void assignColor(Illuminator::HSVs& keys, Illuminator::HSV color);
void assignColor(Illuminator::HSVs& keys, float hue, float saturation, float value);

void assignColor(Illuminator::HSVs& keys, size_t index, Illuminator::HSV color);
void assignColor(Illuminator::HSVs& keys, size_t index, float hue, float saturation, float value);

void resetKeys(Illuminator::HSVs& keys);
void increaseBrightness(Illuminator::HSVs& keys, float step = 0.05);
void decreaseBrightness(Illuminator::HSVs& keys, float step = 0.05);

void RGBtoHSV(float red, float green, float blue, float& hue, float& saturation, float& value);
void HSVtoRGB(float hue, float saturation, float value, float& red, float& green, float& blue);

//----------------------------------------------------------------------------------------------------------------------

}
}
