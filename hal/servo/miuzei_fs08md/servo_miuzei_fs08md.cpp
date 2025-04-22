#include "servo_miuzei_fs08md.h"

namespace RoomScanner
{

ServoMiuzeiFS08MD::ServoMiuzeiFS08MD(uint8_t gpio) 
: m_pwm(PicoCore::PulseWidthModulator(gpio, PWM_FREQUENCY_HZ))
{
}

bool ServoMiuzeiFS08MD::SetTargetAngle(float degrees)
{
    if(degrees < 0 || degrees > UPPER_LIMIT_ANGLE_DEGREES)
    {
        return false;
    }

    // convert the angle to a percentage of the maximum allowable angle
    const float angle_ratio = degrees / UPPER_LIMIT_ANGLE_DEGREES;
    // compute the pulse width length that corrosponds to the angle ratio
    const std::chrono::microseconds pulse_width_length(static_cast<int64_t>(angle_ratio * PULSE_WIDTH_RANGE.count()) + LOWER_LIMIT_PULSE_WIDTH_LENGTH.count());
    // compute the duty cycle
    const float duty_cycle = static_cast<float>(pulse_width_length.count()) / static_cast<float>(PWM_PERIOD.count());

    return m_pwm.SetDutyCycle(duty_cycle);
}

float ServoMiuzeiFS08MD::GetUpperLimitAngleDegrees()
{
    return UPPER_LIMIT_ANGLE_DEGREES;
}
}