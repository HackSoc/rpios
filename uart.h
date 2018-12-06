#pragma once
#include <stdint.h>

#define UART_BASE ((volatile uint32_t *) 0x3F201000)
// Data Register
#define UART_DR (UART_BASE + 0)
//Recieve Status Register
#define UART_RSRECR (UART_BASE + 1)
//Flag Register
#define UART_FR (UART_BASE + 6)
//Integer Baud Rate Divisor
#define UART_IBRD (UART_BASE + 9)
//Fractional Baud Rate divisor
#define UART_FBRD (UART_BASE + 10)
//Line Control Register
#define UART_LCRH (UART_BASE + 11)
//Control Register
#define UART_CR (UART_BASE + 12)

// Interrupt Mask Set Clear Register
#define UART_IMSC (UART_BASE + 14)
// Raw Interupt Status Register 
#define UART_RIS (UART_BASE + 15)
// Masked Interupt Status Register 
#define UART_MIS (UART_BASE + 16)
// Interupt Clear Register
#define UART_ICR (UART_BASE + 17)

void uart_init();
uint8_t uart_read();
void uart_write(uint8_t);
