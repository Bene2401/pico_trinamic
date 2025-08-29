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


TMC_uart_write_datagram_t *tmc_uart_read(trinamic_motor_t driver, TMC_uart_read_datagram_t *dgr)
{
    static TMC_uart_write_datagram_t wdgr = {0};
    uint8_t buffer[8] = {0};
    absolute_time_t start;
    int count = 0;

    // 1. RX-FIFO vorher leeren
    while (uart_is_readable(tmc_uart)) {
        uart_getc(tmc_uart);
    }

    // 2. Telegramm senden
    uart_write_blocking(tmc_uart, dgr->data, sizeof(TMC_uart_write_datagram_t));
    uart_tx_wait_blocking(tmc_uart);

    // 3. Echo prüfen und nur die gesendeten Bytes verwerfen
    while (uart_is_readable(tmc_uart) && count < sizeof(TMC_uart_write_datagram_t)) {
        uint8_t b = uart_getc(tmc_uart);
        if (b != dgr->data[count]) {
            // Antwort fängt hier an
            buffer[0] = b;
            count = 1;
            break;
        }
        count++;
    }

    // 4. Rest der Antwort einlesen (Byte-für-Byte)
    start = get_absolute_time();
    while (count < 8) {
        if (uart_is_readable(tmc_uart)) {
            buffer[count++] = uart_getc(tmc_uart);
        }
        // Timeout 10 ms
        if (to_ms_since_boot(get_absolute_time()) - to_ms_since_boot(start) > 10) {
            wdgr.msg.addr.value = 0xFE; // Timeout
            return &wdgr;
        }
    }

    // 5. Bytes ins Rückgabe-Datagramm kopieren
    for (int i = 0; i < 8; i++) {
        wdgr.data[i] = buffer[i];
    }

    return &wdgr;
}


void tmc_uart_write (trinamic_motor_t driver, TMC_uart_write_datagram_t *dgr)
{
    
    uart_write_blocking(tmc_uart, dgr->data, sizeof(TMC_uart_write_datagram_t));
    uart_tx_wait_blocking(tmc_uart);
}

void tmc_uart_init (uart_inst_t * uart, uint baudrate)
{   
    uart_init(uart, baudrate);
    tmc_uart = uart;
}
