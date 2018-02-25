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

Illuminator::~Illuminator() = default;

//----------------------------------------------------------------------------------------------------------------------

Illuminator::RGB Illuminator::getKeyColor(size_t index)
{
    RGB color;

    if (index < 128)
    {
        const KeyColorModule& key = m_keys[index];
        IlluminatorHelpers::HSVtoRGB(key.getHue(), key.getSaturation(), key.getBrightness(), color.r, color.g, color.b);
    }

    return color;
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::update()
{
    std::for_each(m_keys.begin(), m_keys.end(), [](KeyColorModule& key)
    {
        key.onTimer();
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
        if (index < 0 && index >= 128)
        {
            return;
        }

        KeyColorModule& key = self->m_keys[index];
        if (midiMessage.isNoteOn() && index >= 0 && index < 128)
        {
            const float value = std::clamp<float>(midiMessage.getVelocity() / 127., 0, 1);
            switch(midiMessage.getChannel())
            {
            case 0:
                key.setBrightness(value);
                key.noteOn();
                break;
            case 1:
                key.setHue(value * 360.);
                break;
            case 2:
                key.setSaturation(value);
                break;
            case 3:
                key.setAttack(value * 50.);
                break;
            case 4:
                key.setRelease(value * 127.);
                break;
            }
        }
        else if (midiMessage.isNoteOff() && midiMessage.getChannel() == 0)
        {
            key.noteOff();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

}
