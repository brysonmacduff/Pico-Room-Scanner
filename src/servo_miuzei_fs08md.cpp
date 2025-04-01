#include "servo_miuzei_fs08md.h"

namespace RoomScanner
{

ServoMiuzeiFS08MD::ServoMiuzeiFS08MD(float upper_limit_degrees, uint8_t gpio, uint32_t pwm_frequency_hz) 
: IServo(upper_limit_degrees)
, m_upper_limit_degrees(upper_limit_degrees)
, m_pwm(PicoCore::PulseWidthModulator(gpio,pwm_frequency_hz))
{
}

ServoMiuzeiFS08MD::SetTargetAngle(float degrees)
{
}


}