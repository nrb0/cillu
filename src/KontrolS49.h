#pragma once

#include <memory>

#include <cabl/cabl.h>

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
    std::unique_ptr<Illuminator> m_illuminator;
};

} // namespace cillu
