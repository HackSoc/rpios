#include <stdbool.h>
#include "uart.h"
#include "gpio.h"

void uart_init()
{
	gpio_set_pull(14, 0); // disable pullup/pulldown resistors
	gpio_set_pull(15, 0);
	gpio_set_alt_function(14, 4); // set to alt0 aka main uart
	gpio_set_alt_function(15, 4);

    *UART_CR = 0; // disable uart
    *UART_IBRD = 26; // hardcoded to 115200 baud
    *UART_FBRD = 3;

   	*UART_ICR = 0x7ff; // clear all pending interrupts
    *UART_LCRH = (1 << 4) | (1 << 5) | (1 << 6); // set to 8 bits, 1 stop bit, no parity. also enable FIFOs
    *UART_IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10); // mask all valid interrupts
    *UART_CR = (1 << 0) | (1 << 8) | (1 << 9); // enable uart, enable tx, enable rx.
}

void uart_write(uint8_t c)
{
    while(*UART_FR & (1 << 5)); // wait while tx fifo is full
    *UART_DR = c;
}

uint8_t uart_read()
{
    while(*UART_FR & (1 << 4)); // wait while rx fifo is empty
    return *UART_DR;
}

bool uart_read_ready()
{
    return !(*UART_FR & (1 << 4));
}
