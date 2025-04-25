#include "scanner.h"
#include "servo_miuzei_fs08md.h"
#include "lidar_tf_luna.h"
#include "pico_sleeper.h"
#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

using namespace RoomScanner;

// Servos
static constexpr uint8_t AZIMUTH_SERVO_GPIO = 5;
static constexpr uint8_t ELEVATION_SERVO_GPIO = 6;
static constexpr std::chrono::milliseconds RESET_WAIT_PERIOD {3000};

// Lidar UART
#define UART_ID uart0
static constexpr uint8_t UART_TX_GPIO = 0;
static constexpr uint8_t UART_RX_GPIO = 1;

// Scanner
static constexpr std::chrono::milliseconds SCAN_INTERVAL {1000};
static constexpr float AZIMUTH_INTERVAL_DEGREES = 15;
static constexpr float ELEVATION_INTERVAL_DEGREES = 15;

int main()
{
    cyw43_arch_init();
    stdio_init_all();

    PicoSleeper pico_sleeper;

    LidarTfLuna lidar(UART_ID, UART_TX_GPIO, UART_RX_GPIO);
    const bool is_lidar_ready = lidar.InitializeUart();

    if(!is_lidar_ready)
    {
        return 1;
    }

    ServoMiuzeiFS08MD azimuth_servo(AZIMUTH_SERVO_GPIO);
    ServoMiuzeiFS08MD elevation_servo(ELEVATION_SERVO_GPIO);

    azimuth_servo.SetTargetAngle(0);
    elevation_servo.SetTargetAngle(0);

    // Wait for the servos to return to their zero-angle positions
    sleep_ms(RESET_WAIT_PERIOD.count());

    Scanner scanner(lidar,azimuth_servo,elevation_servo,pico_sleeper,SCAN_INTERVAL,AZIMUTH_INTERVAL_DEGREES,ELEVATION_INTERVAL_DEGREES);

    // Enable the LED to indicate the scan sequence is ready to begin
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    printf("main() -> Starting scanning sequence\n");
    
    scanner.Run();

    return 0;
}