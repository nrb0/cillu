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

    void setKeyHue(unsigned index, float value);
    void setKeySaturation(unsigned index, float value);
    void setKeyBrightness(unsigned index, float value);
    void setKeyFadeIn(unsigned index, float value);
    void setKeyFadeOut(unsigned index, float value);
    void gateKey(unsigned index, bool enabled);

    std::unique_ptr<RtMidiIn> m_midiIn;
    std::array<KeyColorModule, 128> m_modules;
};

//----------------------------------------------------------------------------------------------------------------------

}
