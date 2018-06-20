#include "KeyboardLayer.h"

#include "IlluminatorHelpers.h"
#include "MidiMessage.h"

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

        KeyColorModule& key = self->m_modules[index];
        if (midiMessage.isNoteOn() && index >= 0 && index < 128)
        {
            const unsigned value = std::clamp<float>(midiMessage.getVelocity() / 127., 0, 1);
            if (channel == 0)
            {
                key.gate(true);
            }
            else if (channel == 1)
            {
                key.setRed(value);
            }
            else if (channel == 2)
            {
                key.setGreen(value);
            }
            else if (channel == 3)
            {
                key.setBlue(value);
            }
            else if (channel == 4)
            {
                key.setAttack(value * 127.);
            }
            else if (channel == 5)
            {
                key.setRelease(value * 127.);
            }
        }
        else if (midiMessage.isNoteOff() && channel == 0)
        {
            key.gate(false);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

}
