#ifndef RPIOS_MAILBOX_H
#define RPIOS_MAILBOX_H
#pragma once

#include <stdint.h>
#include "../integration/platform_specific.h"

#define MBOX_BASE ((volatile address_t *const)0x3F00B880)

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

#define PROP_BUFFER_SIZE 256
static uint32_t mbox_prop_buffer[PROP_BUFFER_SIZE] __attribute__((aligned(16)));

uint32_t mbox_read(uint8_t chan);
void mbox_write(uint32_t data, uint8_t chan);
uint8_t* mbox_prop_get(uint32_t channel, uint32_t tag, uint32_t request_bytes, uint8_t* request, uint32_t response_bytes);

#endif
