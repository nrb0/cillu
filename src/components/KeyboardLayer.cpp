#include "KeyboardLayer.h"

#include "IlluminatorHelpers.h"
#include "MidiMessage.h"

#include <utils/ColorHelpers.h>

#include <cabl/util/Log.h>

#include <boost/range/irange.hpp>

#include <RtMidi.h>

#include <algorithm>
#include <iostream>
#include <functional>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

KeyboardLayer::KeyboardLayer(const std::string& midiPortName)
: m_midiIn(std::make_unique<RtMidiIn>())
{
    try
    {
        m_midiIn->openVirtualPort(midiPortName);
        m_midiIn->setCallback(&KeyboardLayer::onMidiMessage, this);
    }
    catch (RtMidiError& error)
    {
        std::string strError = "[KeyboardLayer] Error trying to open " + midiPortName + ": " + error.getMessage();
        M_LOG(strError);
    }
}

//----------------------------------------------------------------------------------------------------------------------

Color KeyboardLayer::getColor(const unsigned index)
{
    if (index < m_modules.size())
    {
        KeyColorModule& key = m_modules[index];
        return key.getColor();
    }

    return Color();
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::update()
{
    std::for_each(m_modules.begin(), m_modules.end(), [](KeyColorModule& key)
    {
        key.update();
    });
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::onMidiMessage(double, std::vector<unsigned char>* message, void* userData)
{
    KeyboardLayer* self = static_cast<KeyboardLayer*>(userData);
    if (!self || !message)
    {
        return;
    }

    const MidiMessage midiMessage (*message);
    if (midiMessage.isNote())
    {
        const int index = midiMessage.getKeyNumber();
        const int channel = midiMessage.getChannel();

        if (midiMessage.isNoteOn())
        {
            const unsigned value = std::clamp<float>(midiMessage.getVelocity() / 127., 0, 1);
            switch(channel)
            {
            case 0:
                self->gateKey(index, true);
                break;
            case 1:
                self->setKeyHue(index, value);
                break;
            case 2:
                self->setKeySaturation(index, value);
                break;
            case 3:
                self->setKeyBrightness(index, value);
                break;
            case 4:
                self->setKeyFadeIn(index, value);
                break;
            case 5:
                self->setKeyFadeOut(index, value);
                break;
            default:
                break;
            }
        }
        else if (midiMessage.isNoteOff() && channel == 0)
        {
            self->gateKey(index, false);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::setKeyHue(const unsigned index, const float newHue)
{
    if (index >= m_modules.size()) return;
    KeyColorModule& module = m_modules[index];
    Color color = module .getColor();

    float hue, saturation, value;

    ColorHelpers::computeHSV(color, hue, saturation, value);
    Color newColor = ColorHelpers::fromHSV(newHue, saturation, value);
    module.setColor(newColor);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::setKeySaturation(const unsigned index, const float newSaturation)
{
    if (index >= m_modules.size()) return;
    KeyColorModule& module = m_modules[index];
    Color color = module .getColor();

    float hue, saturation, value;

    ColorHelpers::computeHSV(color, hue, saturation, value);
    Color newColor = ColorHelpers::fromHSV(hue, newSaturation, value);
    module.setColor(newColor);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::setKeyBrightness(const unsigned index, const float newValue)
{
    if (index >= m_modules.size()) return;
    KeyColorModule& module = m_modules[index];
    Color color = module .getColor();

    float hue, saturation, value;

    ColorHelpers::computeHSV(color, hue, saturation, value);
    Color newColor = ColorHelpers::fromHSV(hue, saturation, newValue);
    module.setColor(newColor);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::setKeyFadeIn(const unsigned index, const float value)
{
    if (index >= m_modules.size()) return;
    KeyColorModule& module = m_modules[index];
    module.setAttack(value);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::setKeyFadeOut(const unsigned index, const float value)
{
    if (index >= m_modules.size()) return;
    KeyColorModule& module = m_modules[index];
    module.setRelease(value);
}

//----------------------------------------------------------------------------------------------------------------------

void KeyboardLayer::gateKey(const unsigned index, const bool enabled)
{
    if (index >= m_modules.size()) return;
    KeyColorModule& module = m_modules[index];
    module.gate(enabled);
}

//----------------------------------------------------------------------------------------------------------------------

}
