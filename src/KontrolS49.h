#pragma once

#include <components/KeyboardLayer.h>

#include <cabl/cabl.h>

#include <memory>
#include <unordered_map>
#include <vector>

class MidiMessage;

namespace cillu
{

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
    KeyboardLayer m_background;
    KeyboardLayer m_foreground;
};

} // namespace cillu
