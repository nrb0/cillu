#pragma once

#include "KeyColorModule.h"

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

    Illuminator::RGB getKeyColor(size_t index, bool isForeground);

    void update();

private:
    static void onMidiMessage(double timeStamp, std::vector<unsigned char>* message, void* userData);

    std::unique_ptr<RtMidiIn> m_midiIn;

    KeyColorModules m_background;
    KeyColorModules m_foreground;
};

//----------------------------------------------------------------------------------------------------------------------

}
