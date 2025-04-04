#include "lidar_tf_luna.h"
#include <chrono>
#include "pico/cyw43_arch.h"

using namespace RoomScanner;

#define UART_ID uart0
static constexpr uint8_t UART_TX_GPIO = 0;
static constexpr uint8_t UART_RX_GPIO = 1;
static constexpr std::chrono::milliseconds RX_PRINT_DELAY {100};

int main()
{
    cyw43_arch_init();
    stdio_init_all();

    LidarTfLuna lidar_sensor(UART_ID, UART_TX_GPIO, UART_RX_GPIO);
    lidar_sensor.InitializeUart();

    bool led_toggle = true;

    while(true)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_toggle);
        TfLunaSensorData data = lidar_sensor.GetSensorData();
        const uint32_t distance_cm = lidar_sensor.GetDistance();
        printf("main() -> GetSensorData: {%u cm, %u celcius, %u}, GetDistance: {%u}\n",data.distance_cm,data.temperature_c,data.signal_strength,distance_cm);
        sleep_ms(RX_PRINT_DELAY.count());
        led_toggle = !led_toggle;
    }
    
    return 0;
}