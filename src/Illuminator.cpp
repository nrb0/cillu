#include "Illuminator.h"
#include "IlluminatorHelpers.h"
#include "MidiMessage.h"

#include <cabl/util/Log.h>

#include <boost/range/irange.hpp>

#include <RtMidi.h>

#include <algorithm>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

Illuminator::Illuminator()
: m_midiIn(std::make_unique<RtMidiIn>())
{
    try
    {
        m_midiIn->openVirtualPort("cillu");
        m_midiIn->setCallback(&Illuminator::onMidiMessage, this);
    }
    catch (RtMidiError& error)
    {
        std::string strError = "[Illuminator] RtMidiError: " + error.getMessage();
        M_LOG(strError);
    }
}

//----------------------------------------------------------------------------------------------------------------------

Color Illuminator::getKeyColor(const size_t index, const bool isForeground)
{
    if (index < 128)
    {
        KeyColorModule& key = isForeground ? m_foreground[index] : m_background[index];
        return key.getColor();
    }

    return Color();
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::update()
{
    std::for_each(m_background.begin(), m_background.end(), [](KeyColorModule& key)
    {
        key.update();
    });

    std::for_each(m_foreground.begin(), m_foreground.end(), [](KeyColorModule& key)
    {
        key.update();
    });
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onMidiMessage(double, std::vector<unsigned char>* message, void* userData)
{
    Illuminator* self = static_cast<Illuminator*>(userData);
    if (!self || !message)
    {
        return;
    }

    const MidiMessage midiMessage (*message);
    if (midiMessage.isNote())
    {
        const int index = midiMessage.getKeyNumber();
        const int channel = midiMessage.getChannel();

        if (index < 0 && index >= 128)
        {
            return;
        }

        KeyColorModule& key = channel < 6 ? self->m_foreground[index] : self->m_background[index];
        if (midiMessage.isNoteOn() && index >= 0 && index < 128)
        {
            const unsigned value = std::clamp<float>(midiMessage.getVelocity() / 127., 0, 1);
            if (channel == 0 || channel == 6)
            {
                key.gate(true);
            }
            else if (channel == 1 || channel == 7)
            {
                key.setRed(value);
            }
            else if (channel == 2 || channel == 8)
            {
                key.setGreen(value);
            }
            else if (channel == 3 || channel == 9)
            {
                key.setBlue(value);
            }
            else if (channel == 4 || channel == 10)
            {
                key.setAttack(value * 127.);
            }
            else if (channel == 5 || channel == 11)
            {
                key.setRelease(value * 127.);
            }
        }
        else if (midiMessage.isNoteOff() && (channel == 0 || channel == 6))
        {
            key.gate(false);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

}
