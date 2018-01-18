#pragma once

#include "KeyColorModule.h"

#include <osc/OscPacketListener.h>

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class RtMidiIn;

namespace cillu
{

class OSCReceiver;

//----------------------------------------------------------------------------------------------------------------------

class Illuminator
{
public:

    struct HSV
    {
        float h;
        float s;
        float v;
    };

    struct RGB
    {
        float r;
        float g;
        float b;
    };

    using HSVs = std::array<HSV, 128>;
    using KeyColorModules = std::array<KeyColorModule, 128>;

    Illuminator();
    ~Illuminator();

    Illuminator::RGB getKeyColor(size_t index);

    void update();

private:
    void onOscMessage(const osc::ReceivedMessage& msg);
    static void onMidiMessage(double timeStamp, std::vector<unsigned char>* message, void* userData);

    void onReceiveNoteMessage(const osc::ReceivedMessage& msg);

    void onReceiveNoteHueMessage(const osc::ReceivedMessage& msg);
    void onReceiveNoteSatMessage(const osc::ReceivedMessage& msg);
    void onReceiveNoteValMessage(const osc::ReceivedMessage& msg);

    void onReceiveEGAttackMessage(const osc::ReceivedMessage& msg);
    void onReceiveEGDecayMessage(const osc::ReceivedMessage& msg);
    void onReceiveEGReleaseMessage(const osc::ReceivedMessage& msg);

    std::unique_ptr<OSCReceiver> m_oscReceiver;
    std::unique_ptr<RtMidiIn> m_midiIn;

    KeyColorModules m_keys;

    std::unordered_map<std::string, std::function<void(const osc::ReceivedMessage&)>> m_messageMap;
};

//----------------------------------------------------------------------------------------------------------------------

}
