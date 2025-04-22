#pragma once

#include "i_servo.h"
#include <pulse_width_modulator.h>
#include <chrono>

namespace RoomScanner
{
class ServoMiuzeiFS08MD : public IServo
{
public:

    static constexpr float UPPER_LIMIT_ANGLE_DEGREES = 180;

    ServoMiuzeiFS08MD(uint8_t gpio);
    ~ServoMiuzeiFS08MD() = default;
    bool SetTargetAngle(float degrees) override;
    float GetUpperLimitAngleDegrees() override;

protected:

    static constexpr uint32_t PWM_FREQUENCY_HZ = 50;
    // The PWM period is the inverse of the PWM signal frequency, expressed in microseconds
    static constexpr std::chrono::microseconds PWM_PERIOD {20000};
    static constexpr std::chrono::microseconds LOWER_LIMIT_PULSE_WIDTH_LENGTH {500};
    static constexpr std::chrono::microseconds UPPER_LIMIT_PULSE_WIDTH_LENGTH {2500};
    // The pulse width range is the absolute difference of the upper and lower limit pulse width lengths
    static constexpr std::chrono::microseconds PULSE_WIDTH_RANGE {2000};

    PicoCore::PulseWidthModulator m_pwm;

};
}