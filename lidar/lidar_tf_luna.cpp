#include "lidar_tf_luna.h"


namespace RoomScanner
{
LidarTfLuna::LidarTfLuna(uart_inst_t* uart_instance, uint8_t uart_tx_gpio, uint8_t uart_rx_gpio)
: m_uart_id(uart_instance), m_uart_tx_gpio(uart_tx_gpio), m_uart_rx_gpio(uart_rx_gpio)
{
}

uint32_t LidarTfLuna::GetDistance()
{
    if(m_uart_id == uart0)
    {
        return uart0_sensor_data.distance_cm;
    }
    else
    {
        return uart1_sensor_data.distance_cm;
    }
}

bool LidarTfLuna::InitializeUart()
{
    // Initialize UART
    uart_init(m_uart_id, UART_BAUD_RATE);
    gpio_set_function(m_uart_tx_gpio, UART_FUNCSEL_NUM(m_uart_id, m_uart_tx_gpio));
    gpio_set_function(m_uart_rx_gpio, UART_FUNCSEL_NUM(m_uart_id, m_uart_rx_gpio));

    uart_set_hw_flow(m_uart_id, false, false);
    uart_set_format(m_uart_id, UART_DATA_BITS, UART_STOP_BITS, UART_PARITY);
    uart_set_fifo_enabled(m_uart_id, false);    

    // get the interrupt id that matches the chosen uart id
    const int uart_irq = m_uart_id == uart0 ? UART0_IRQ : UART1_IRQ;

    // Set up and enable the interrupt handlers
    if(m_uart_id == uart0)
    {
        irq_set_exclusive_handler(uart_irq, UartZeroRxHandler);
    }
    else
    {
        irq_set_exclusive_handler(uart_irq, UartOneRxHandler);
    }

    irq_set_enabled(uart_irq, true);
    uart_set_irq_enables(m_uart_id, true, false);

    return true;
}

LidarTfLuna::SensorData LidarTfLuna::GetSensorData() const
{
    if(m_uart_id == uart0)
    {
        return uart0_sensor_data;
    }
    else
    {
        return uart1_sensor_data;
    }
}

void LidarTfLuna::UartZeroRxHandler()
{
    while (uart_is_readable(uart0)) 
    {
        uint8_t buffer[UART_RX_BUFFER_SIZE];
        uart_read_blocking(uart0, buffer, UART_RX_BUFFER_SIZE);

        if(buffer[0] != UART_DATA_START_BYTE || buffer[1] != UART_DATA_START_BYTE)
        {
            return;
        }

        uart0_sensor_data.distance_cm = buffer[2] + (buffer[3] << UART_SHIFT_BITS);
        uart0_sensor_data.signal_strength = buffer[4] + (buffer[5] << UART_SHIFT_BITS);
        // Note that the raw temperature value is meaningless without division by 8 and subtraction by 256, according to the TF Luna documentation.
        uart0_sensor_data.temperature_c = (buffer[6] + (buffer[7] << UART_SHIFT_BITS)) / UART_TEMPERATURE_DIVISOR - UART_TEMPERATURE_OFFSET;

        printf("LidarTfLuna::UartRxHandler() -> RX: {%d cm, %d, %d celcius}\n",uart0_sensor_data.distance_cm,uart0_sensor_data.signal_strength,uart0_sensor_data.temperature_c);
    }
}

void LidarTfLuna::UartOneRxHandler()
{
    while (uart_is_readable(uart1)) 
    {
        uint8_t buffer[UART_RX_BUFFER_SIZE];
        uart_read_blocking(uart1, buffer, UART_RX_BUFFER_SIZE);

        if(buffer[0] != UART_DATA_START_BYTE || buffer[1] != UART_DATA_START_BYTE)
        {
            return;
        }

        uart1_sensor_data.distance_cm = buffer[2] + (buffer[3] << UART_SHIFT_BITS);
        uart1_sensor_data.signal_strength = buffer[4] + (buffer[5] << UART_SHIFT_BITS);
        // Note that the raw temperature value is meaningless without division by 8 and subtraction by 256, according to the TF Luna documentation.
        uart1_sensor_data.temperature_c = (buffer[6] + (buffer[7] << UART_SHIFT_BITS)) / UART_TEMPERATURE_DIVISOR - UART_TEMPERATURE_OFFSET;

        printf("LidarTfLuna::UartRxHandler() -> RX: {%d cm, %d, %d celcius}\n",uart1_sensor_data.distance_cm,uart1_sensor_data.signal_strength,uart1_sensor_data.temperature_c);
    }
}
}