#pragma once

#include "KeyColorModule.h"

#include <utils/Color.h>

#include <array>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class RtMidiIn;

namespace cillu
{

//----------------------------------------------------------------------------------------------------------------------

class Illuminator
{
public:
    using KeyColorModules = std::array<KeyColorModule, 128>;

    Illuminator();

    void update();
    Color getKeyColor(const size_t index, const bool isForeground = true);

private:
    static void onMidiMessage(double timeStamp, std::vector<unsigned char>* message, void* userData);

    std::unique_ptr<RtMidiIn> m_midiIn;

    KeyColorModules m_background;
    KeyColorModules m_foreground;
};

//----------------------------------------------------------------------------------------------------------------------

}
