#include "i_servo.h"
#include "pulse_width_modulator.h"

namespace RoomScanner
{
class ServoMiuzeiFS08MD : public IServo
{
public:
    ServoMiuzeiFS08MD(float upper_limit_degrees, uint8_t gpio, uint32_t pwm_frequency_hz);
    ~ServoMiuzeiFS08MD() = default;
    SetTargetAngle(float degrees) override;
protected:
    PicoCore::PulseWidthModulator m_pwm;
    float m_upper_limit_degrees;
};
}