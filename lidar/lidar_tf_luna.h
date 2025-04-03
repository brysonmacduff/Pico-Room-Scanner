#include "i_lidar.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdlib.h>
#include <stdio.h>

namespace RoomScanner
{
// The Raspberry Pi Pico only supports two UART instances. Therefore, this class contains a static UART RX handler for each UART instance.
// Similarly, the static UART RX handlers deposit the received data into a pair of corrosponding static structs.
class LidarTfLuna : public ILidar
{
public:

    struct SensorData
    {
        uint32_t distance_cm;
        uint32_t temperature_c;
        uint32_t signal_strength;
    };

    ~LidarTfLuna() = default;
    // Note that this class does not validate that the provided UART arguments are valid. Consult the pico pin layout for more information.
    LidarTfLuna(uart_inst_t* uart_instance, uint8_t uart_tx_gpio, uint8_t uart_rx_gpio);
    // Returns the distance in centimeters by default
    uint32_t GetDistance() override;
    bool InitializeUart();
    SensorData GetSensorData() const;

protected:

    // Note that UART RX message data is formatted as follows: 
    // [START,START,DISTANCE(7-0),DISTANCE(15-8),STRENGTH(7-0),STRENGTH(15-8),TEMPERATURE(7-0),TEMPERATURE(15-8),CHECKSUM]
    // Note that 16 bit integers, such as distance, are spread across to sequential bytes, requiring the second byte to be shifted left by 8 bits
    // because it represents the upper 8 bits of the combined 16 bit integer

    // These are the UART configuration constants that are compatible with the TF Luna specifications.
    static constexpr uint32_t UART_BAUD_RATE = 115200;
    static constexpr uint8_t UART_DATA_BITS = 8;
    static constexpr uint8_t UART_STOP_BITS = 1;
    static constexpr uart_parity_t UART_PARITY = UART_PARITY_NONE;
    static constexpr uint8_t UART_DATA_START_BYTE = 0x59;
    static constexpr uint8_t UART_SHIFT_BITS = 8;
    static constexpr size_t UART_RX_BUFFER_SIZE = 9;
    static constexpr uint32_t UART_TEMPERATURE_DIVISOR = 8;
    static constexpr uint32_t UART_TEMPERATURE_OFFSET = 256;
    
    // Sensor data will be saved in the static struct corrosponding to m_uart_id (UART instance zero or one)
    static SensorData uart0_sensor_data;
    static SensorData uart1_sensor_data;

    uart_inst_t* m_uart_id;
    uint8_t m_uart_tx_gpio;
    uint8_t m_uart_rx_gpio;
    
    // The UART RX handler function that will be used corrosponds to m_uart_id (UART instance zero or one)
    static void UartZeroRxHandler();
    static void UartOneRxHandler();
};
}