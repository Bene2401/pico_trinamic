#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "trinamic.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define UART_BAUD 230400

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 8
#define UART_RX_PIN 9

int main()
{
    stdio_init_all();

    // Set up our UART
    tmc_uart_init(UART_ID, UART_BAUD);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_init(2);
    gpio_init(3);
    gpio_set_dir(2, GPIO_OUT);
    gpio_set_dir(3, GPIO_OUT);
    gpio_put(2, 1); // LED ON
    gpio_put(3, 1); // LED ON

    while (!stdio_usb_connected()) {
    sleep_ms(100);
    }
      printf("Hallo über USB!\n");
    

    TMC2209_t driver = {0};
    TMC2209_SetDefaults(&driver);
    printf("1!\n");
    TMC2209_Init(&driver);
    printf("2!\n");
    TMC2209_SetCurrent(&driver, 500, 50);
    printf("init_abgeschlossen!\n");

    while (true) {
        uint16_t current = TMC2209_GetCurrent(&driver, TMCCurrent_Actual);
        printf("%d\n", current);
        sleep_ms(1000);
    }
}
