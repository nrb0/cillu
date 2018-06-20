#pragma once

#include <components/KeyColorModule.h>

#include <utils/Color.h>

#include <array>
#include <vector>

class RtMidiIn;

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

class KeyboardLayer
{
public:
    KeyboardLayer(const std::string& midiPortName);

    void update();

    Color getColor(const unsigned index);

private:
    static void onMidiMessage(double timestamp, std::vector<unsigned char>* message, void* userData);

    std::unique_ptr<RtMidiIn> m_midiIn;
    std::array<KeyColorModule, 128> m_modules;
};

//----------------------------------------------------------------------------------------------------------------------

}
