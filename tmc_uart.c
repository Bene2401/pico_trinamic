/*
  tmc_uart.c - driver code for RP2040 ARM processors

  Copyright (c) 2025 Hannah Eikens
  Original code retrieved from https://github.com/grblHAL/RP2040/blob/master/tmc_uart.c

  grblHAL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  grblHAL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with grblHAL. If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"
#include "tmc_uart.h"



static uart_inst_t * tmc_uart;


TMC_uart_write_datagram_t *tmc_uart_read(trinamic_motor_t motor, TMC_uart_read_datagram_t *datagram)
{
    static TMC_uart_write_datagram_t rx_buffer;
    static uart_inst_t *last_uart = NULL;
    static uint last_baud = 0;

    // UART nur initialisieren, wenn sich UART oder Baudrate geändert hat
    if (last_uart != motor.UARTID || last_baud != motor.UART_BAUD)
    {
        tmc_uart_init(motor.UARTID, motor.UART_BAUD);
        last_uart = motor.UARTID;
        last_baud = motor.UART_BAUD;
    }

    // Datagram senden
    for (int i = 0; i < sizeof(TMC_uart_read_datagram_t); i++)
    {
        uart_putc_raw(motor.UARTID, datagram->data[i]);
    }

    // Antwort empfangen (8 Bytes für TMC2209)
    for (int i = 0; i < 8; i++)
    {
        rx_buffer.data[i] = uart_getc(motor.UARTID);
    }

    return &rx_buffer;
}




void tmc_uart_write(trinamic_motor_t motor, TMC_uart_write_datagram_t *datagram)
{
    static uart_inst_t *last_uart = NULL;
    static uint last_baud = 0;

    // UART nur initialisieren, wenn sich UART oder Baudrate geändert hat
    if (last_uart != motor.UARTID || last_baud != motor.UART_BAUD)
    {
        tmc_uart_init(motor.UARTID, motor.UART_BAUD);
        last_uart = motor.UARTID;
        last_baud = motor.UART_BAUD;
    }

    for (int i = 0; i < sizeof(TMC_uart_write_datagram_t); i++)
    {
        uart_putc_raw(motor.UARTID, datagram->data[i]);
    }

    // Optional: kleine Pause
    sleep_ms(1);
}


void tmc_uart_init (uart_inst_t * uart, uint baudrate)
{   
    uart_init(uart, baudrate);
    //tmc_uart = uart;
}
