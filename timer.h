#ifndef _TIMER_H
#define _TIMER_H
#pragma once

#include <stdint.h>

#define TIMER_BASE 0x3F003000

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
