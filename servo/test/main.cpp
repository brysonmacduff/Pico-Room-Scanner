#include "servo_miuzei_fs08md.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

using namespace RoomScanner;

static constexpr uint8_t PWM_GPIO = 5;
static constexpr std::chrono::milliseconds SERVO_INTERVAL {1000};

static constexpr float BOTTOM_ANGLE_DEGREES = 0;
static constexpr float MIDDLE_ANGLE_DEGREES = 90;
static constexpr float TOP_ANGLE_DEGREES = 180;

int main()
{
    stdio_init_all();
    cyw43_arch_init();

    ServoMiuzeiFS08MD servo(PWM_GPIO);

    bool set_angle_result = false;
    bool led_toggle = true;

    while(true)
    {
        set_angle_result = servo.SetTargetAngle(BOTTOM_ANGLE_DEGREES);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
        led_toggle = !led_toggle;
        printf("main() -> Setting target angle to: {%f}\n",BOTTOM_ANGLE_DEGREES);
        sleep_ms(SERVO_INTERVAL.count());

        set_angle_result = servo.SetTargetAngle(MIDDLE_ANGLE_DEGREES);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
        led_toggle = !led_toggle;
        printf("main() -> Setting target angle to: {%f}\n",MIDDLE_ANGLE_DEGREES);
        sleep_ms(SERVO_INTERVAL.count());

        set_angle_result = servo.SetTargetAngle(TOP_ANGLE_DEGREES);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
        led_toggle = !led_toggle;
        printf("main() -> Setting target angle to: {%f}\n",TOP_ANGLE_DEGREES);
        sleep_ms(SERVO_INTERVAL.count());

        set_angle_result = servo.SetTargetAngle(MIDDLE_ANGLE_DEGREES);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
        led_toggle = !led_toggle;
        printf("main() -> Setting target angle to: {%f}\n",MIDDLE_ANGLE_DEGREES);
        sleep_ms(SERVO_INTERVAL.count());
    }

    return 0;
}