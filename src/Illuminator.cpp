#include "Illuminator.h"
#include "IlluminatorHelpers.h"
#include "OSCReceiver.h"

#include <algorithm>
#include <iostream>

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

Illuminator::Illuminator()
: m_oscReceiver(std::make_unique<OSCReceiver>(15243, std::bind(&Illuminator::onOscMessage, this, std::placeholders::_1)))
{
    m_messageMap.insert({
        // Transport
        {"/note/hue", std::bind(&Illuminator::onReceiveNoteHueMessage, this, std::placeholders::_1)},
        {"/note/sat", std::bind(&Illuminator::onReceiveNoteSatMessage, this, std::placeholders::_1)},
        {"/note/val", std::bind(&Illuminator::onReceiveNoteValMessage, this, std::placeholders::_1)},
        {"/eg/attack", std::bind(&Illuminator::onReceiveEGAttackMessage, this, std::placeholders::_1)},
        {"/eg/decay", std::bind(&Illuminator::onReceiveEGDecayMessage, this, std::placeholders::_1)},
        {"/eg/release", std::bind(&Illuminator::onReceiveEGReleaseMessage, this, std::placeholders::_1)},
    });

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

void Illuminator::onReceiveNoteHueMessage(const osc::ReceivedMessage& msg)
{
    if (msg.ArgumentCount() < 2)
    {
        return;
    }

    auto arguments = msg.ArgumentsBegin();
    const int noteIndex = (arguments++)->AsInt32();
    const float hue = (arguments++)->AsFloat();

    if (noteIndex >= 0 && noteIndex < 128)
    {
        KeyColorModule& key = m_keys[noteIndex];
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

    auto arguments = msg.ArgumentsBegin();
    const int noteIndex = (arguments++)->AsInt32();
    const float saturation = (arguments++)->AsFloat();

    if (noteIndex >= 0 && noteIndex < 128)
    {
        KeyColorModule& key = m_keys[noteIndex];
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

    auto arguments = msg.ArgumentsBegin();
    const int noteIndex = (arguments++)->AsInt32();
    const float value = (arguments++)->AsFloat();

    if (noteIndex >= 0 && noteIndex < 128)
    {
        KeyColorModule& key = m_keys[noteIndex];
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