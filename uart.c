#include "uart.h"

void uart_init()
{
    *UART_CR = 0;
    *UART_IBRD = 26;
    *UART_FBRD = 3;
    *UART_LCRH = (1 << 4) | (1 << 5) | (1 << 6);
    *UART_IMSC = (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10);
    *UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_write(uint8_t c)
{
    while(*UART_FR & (1 << 5));
    *UART_DR = c;
}

uint8_t uart_read()
{
    while(*UART_FR & (1 << 4));
    return *UART_DR;
}
