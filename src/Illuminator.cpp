#include "Illuminator.h"
#include "IlluminatorHelpers.h"
#include "MidiMessage.h"
#include "OSCReceiver.h"

#include <boost/range/irange.hpp>

#include <RtMidi.h>

#include <algorithm>
#include <iostream>

namespace cillu
{

constexpr char midiPortName[] = "IAC Driver Bus 1";

//----------------------------------------------------------------------------------------------------------------------

Illuminator::Illuminator()
: m_oscReceiver(std::make_unique<OSCReceiver>(15243, std::bind(&Illuminator::onOscMessage, this, std::placeholders::_1)))
, m_midiIn(std::make_unique<RtMidiIn>())
{
    m_messageMap.insert({
        {"/note", std::bind(&Illuminator::onReceiveNoteMessage, this, std::placeholders::_1)},
        {"/note/hue", std::bind(&Illuminator::onReceiveNoteHueMessage, this, std::placeholders::_1)},
        {"/note/sat", std::bind(&Illuminator::onReceiveNoteSatMessage, this, std::placeholders::_1)},
        {"/note/val", std::bind(&Illuminator::onReceiveNoteValMessage, this, std::placeholders::_1)},
        {"/eg/attack", std::bind(&Illuminator::onReceiveEGAttackMessage, this, std::placeholders::_1)},
        {"/eg/decay", std::bind(&Illuminator::onReceiveEGDecayMessage, this, std::placeholders::_1)},
        {"/eg/release", std::bind(&Illuminator::onReceiveEGReleaseMessage, this, std::placeholders::_1)},
    });

    const int numPorts = m_midiIn->getPortCount();
    for (const size_t portIndex : boost::irange<size_t>(0, numPorts))
    {
      try
      {
        const std::string portName = m_midiIn->getPortName(portIndex);
        if (portName.find(midiPortName) != std::string::npos)
        {
          m_midiIn->openPort(portIndex);
        }
      }
      catch (RtMidiError& error)
      {
        std::string strError(error.getMessage());
        std::cout << "[Illuminator] RtMidiError: " << strError;
      }
    }
    if (!m_midiIn->isPortOpen())
    {
      m_midiIn.reset(nullptr);
    }
    else
    {
      m_midiIn->setCallback(&Illuminator::onMidiMessage, this);
    }

    m_oscReceiver->start();
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

void Illuminator::onOscMessage(const osc::ReceivedMessage& msg)
{
    auto it = m_messageMap.find(msg.AddressPattern());
    if (it != m_messageMap.end())
    {
        try
        {
            it->second(msg);
        }
        catch (osc::MissingArgumentException&)
        {
        }
        catch (osc::WrongArgumentTypeException&)
        {
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onMidiMessage(double timeStamp, std::vector<unsigned char>* message, void* userData)
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

void Illuminator::onReceiveNoteMessage(const osc::ReceivedMessage& msg)
{

}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onReceiveNoteHueMessage(const osc::ReceivedMessage& msg)
{
    if (msg.ArgumentCount() < 2)
    {
        return;
    }

    const bool isRange = msg.ArgumentCount() == 3;

    auto arguments = msg.ArgumentsBegin();
    const int noteStart = (arguments++)->AsInt32();
    const int noteEnd = std::min(isRange ? (arguments++)->AsInt32() : noteStart + 1, 128);
    const float hue = (arguments++)->AsFloat();

    if (noteStart < 0 || noteStart >= noteEnd)
    {
        return;
    }

    for (const size_t index : boost::irange<size_t>(noteStart, noteEnd))
    {
        KeyColorModule& key = m_keys[index];
        key.setHue(std::clamp<float>(hue * 360, 0, 360));
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onReceiveNoteSatMessage(const osc::ReceivedMessage& msg)
{
    if (msg.ArgumentCount() < 2)
    {
        return;
    }

    const bool isRange = msg.ArgumentCount() == 3;

    auto arguments = msg.ArgumentsBegin();
    const int noteStart = (arguments++)->AsInt32();
    const int noteEnd = std::min(isRange ? (arguments++)->AsInt32() : noteStart + 1, 128);
    const float saturation = (arguments++)->AsFloat();

    if (noteStart < 0 || noteStart >= noteEnd)
    {
        return;
    }

    for (const size_t index : boost::irange<size_t>(noteStart, noteEnd))
    {
        KeyColorModule& key = m_keys[index];
        key.setSaturation(std::clamp<float>(saturation, 0, 1));
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onReceiveNoteValMessage(const osc::ReceivedMessage& msg)
{
    if (msg.ArgumentCount() < 2)
    {
        return;
    }

    const bool isRange = msg.ArgumentCount() == 3;

    auto arguments = msg.ArgumentsBegin();
    const int noteStart = (arguments++)->AsInt32();
    const int noteEnd = std::min(isRange ? (arguments++)->AsInt32() : noteStart + 1, 128);
    const float value = (arguments++)->AsFloat();

    if (noteStart < 0 || noteStart >= noteEnd)
    {
        return;
    }

    for (const size_t index : boost::irange<size_t>(noteStart, noteEnd))
    {
        KeyColorModule& key = m_keys[index];
        key.setBrightness(std::clamp<float>(value, 0, 1));
    }
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onReceiveEGAttackMessage(const osc::ReceivedMessage& msg)
{
    if (msg.ArgumentCount() < 1)
    {
        return;
    }

    auto arguments = msg.ArgumentsBegin();
    const float value = (arguments++)->AsFloat();

    std::for_each(m_keys.begin(), m_keys.end(), [&value](KeyColorModule& key)
    {
        key.setAttack(value);
    });
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onReceiveEGDecayMessage(const osc::ReceivedMessage& msg)
{
    if (msg.ArgumentCount() < 1)
    {
        return;
    }

    auto arguments = msg.ArgumentsBegin();
    const float value = (arguments++)->AsFloat();

    std::for_each(m_keys.begin(), m_keys.end(), [&value](KeyColorModule& key)
    {
        key.setDecay(value);
    });
}

//----------------------------------------------------------------------------------------------------------------------

void Illuminator::onReceiveEGReleaseMessage(const osc::ReceivedMessage& msg)
{
    if (msg.ArgumentCount() < 1)
    {
        return;
    }

    auto arguments = msg.ArgumentsBegin();
    const float value = (arguments++)->AsFloat();

    std::for_each(m_keys.begin(), m_keys.end(), [&value](KeyColorModule& key)
    {
        key.setRelease(value);
    });
}

//----------------------------------------------------------------------------------------------------------------------

}
