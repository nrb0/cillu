#pragma once

#include <cabl/cabl.h>

#include <memory>
#include <unordered_map>
#include <vector>

class MidiMessage;
class RtMidiOut;

namespace cillu
{
class Illuminator;

class KontrolS49 : public sl::cabl::Client
{
public:
    KontrolS49();
    ~KontrolS49();

    void onTimer();

    void initDevice() override;
    void disconnected() override;

    void render() override;

    void buttonChanged(sl::cabl::Device::Button button_, bool buttonState_, bool shiftState_) override;
    void encoderChanged(unsigned encoder_, bool valueIncreased_, bool shiftPressed_) override;
    void keyChanged(unsigned index_, double value_, bool shiftPressed) override;
    void controlChanged(unsigned pot_, double value_, bool shiftPressed) override;

private:

    enum Parameters
    {
        SCREEN_ENCODERS = 21,
        SCREEN_ENCODER_1 = 21,
        SCREEN_ENCODER_2,
        SCREEN_ENCODER_3,
        SCREEN_ENCODER_4,
        SCREEN_ENCODER_5,
        SCREEN_ENCODER_6,
        SCREEN_ENCODER_7,
        SCREEN_ENCODER_8,
        CHAIN_SELECTOR = 105
    };

    void incDecParameter(unsigned id, bool increment);

    void sendControlChange(unsigned channel, unsigned id, unsigned value);
    void sendMIDIMessage(MidiMessage& message);

    void incDecChainSelector(bool increment);
    void incDecEncoder(unsigned encoder, bool increment);

    std::unique_ptr<Illuminator> m_illuminator;
    std::unique_ptr<RtMidiOut> m_midiOut;

    std::unordered_map<unsigned, unsigned> m_mappingValue;
};

} // namespace cillu
