#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <string>
#include <chrono>
#include <memory>

#define UART_ID uart0 // There are only two uart instance options (UART0 or UART1)
static constexpr uint8_t TX_GPIO = 0; // UART0 TX GPIO PIN
static constexpr uint8_t RX_GPIO = 1; // UART0 RX GPIO PIN
static constexpr uint32_t BAUD_RATE = 115200;
static constexpr uint8_t DATA_BITS = 8;
static constexpr uint8_t STOP_BITS = 1;
static constexpr uart_parity_t PARITY = UART_PARITY_NONE;

static std::chrono::milliseconds TX_DELAY {1000};

// These are instructions (from Pico to TF Luna) to configure the TX Luna
static const char DEFAULT_OUTPUT_FORMAT[] = {0x5A,0x05,0x05,0x02,0x00};
static const char ENABLE_OUTPUT[] = {0x5A,0x05,0x07,0x01,0x00};

std::string rx_message;

// Called via a UART harware interrupt
void UartRxHandler()
{
    while (uart_is_readable(UART_ID)) 
    {
        const size_t buffer_size = 9;
        uint8_t buffer[buffer_size];
        uart_read_blocking(UART_ID, buffer, buffer_size);

        if(buffer[0] != 0x59 || buffer[1] != 0x59)
        {
            return;
        }

        uint16_t distance_cm = buffer[2] + (buffer[3] << 8);
        uint16_t signal_strength = buffer[4] + (buffer[5] << 8);
        uint16_t temperature_c = (buffer[6] + (buffer[7] << 8)) / 8 - 256;

        printf("UartRxHandler() -> RX: {%d cm, %d, %d celcius}\n",distance_cm,signal_strength,temperature_c);

        //printf("UartRxHandler() -> RX: {%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x}\n",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7],buffer[8]);

    }
}

int main()
{
    stdio_init_all();

    uart_inst_t* uart_id = UART_ID;

    uart_init(uart_id, BAUD_RATE);

    gpio_set_function(TX_GPIO, UART_FUNCSEL_NUM(UART_ID, TX_GPIO));
    gpio_set_function(RX_GPIO, UART_FUNCSEL_NUM(UART_ID, RX_GPIO));

    uart_set_hw_flow(uart_id, false, false);
    uart_set_format(uart_id, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(uart_id, false);
    int UART_IRQ = uart_id == uart0 ? UART0_IRQ : UART1_IRQ;

    // Set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, UartRxHandler);
    irq_set_enabled(UART_IRQ, true);
    // Enable the UART to send interrupts - RX only
    uart_set_irq_enables(uart_id, true, false);

    // configure TF Luna output mode
    //uart_puts(uart_id, ENABLE_OUTPUT);
    //uart_puts(uart_id, DEFAULT_OUTPUT_FORMAT);

    while(true)
    {
    }

    return 0;
}