#pragma once
#include <stdint.h>

#define MBOX_BASE ((volatile uint32_t * const) 0x3F00B880)

#define MBOX_R_D (MBOX_BASE + 0x0)
#define MBOX_R_P (MBOX_BASE + 0x4)
#define MBOX_R_E (MBOX_BASE + 0x5)
#define MBOX_R_S (MBOX_BASE + 0x6)
#define MBOX_R_C (MBOX_BASE + 0x7)

#define MBOX_W_D (MBOX_BASE + 0x8)
#define MBOX_W_P (MBOX_BASE + 0xC)
#define MBOX_W_E (MBOX_BASE + 0xD)
#define MBOX_W_S (MBOX_BASE + 0xE)
#define MBOX_W_C (MBOX_BASE + 0xF)

uint32_t mbox_read(uint8_t chan);
void mbox_write(uint32_t data, uint8_t chan);
