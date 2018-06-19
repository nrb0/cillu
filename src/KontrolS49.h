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

    void keyChanged(unsigned index_, double value_, bool shiftPressed) override;

private:

    void sendControlChange(unsigned channel, unsigned id, unsigned value);
    void sendMIDIMessage(MidiMessage& message);

    std::unique_ptr<Illuminator> m_illuminator;
    std::unique_ptr<RtMidiOut> m_midiOut;
};

} // namespace cillu
