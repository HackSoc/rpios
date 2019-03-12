#ifndef RPIOS_FRAMEBUFFER_H
#define RPIOS_FRAMEBUFFER_H
#pragma once

#include <stdint.h>

#include "compiler_specific.h"

struct FrameBufferConfig
{
    uint32_t width;
    uint32_t height;
    uint32_t vwidth;
    uint32_t vheight;
    uint32_t pitch;
    uint32_t depth;
    uint32_t x;
    uint32_t y;
    uint32_t addr;
    uint32_t size;
} ALIGN(16);

volatile uint32_t *framebuffer_init(uint32_t width, uint32_t height);
void framebuffer_set_pixel(uint32_t, uint32_t, uint32_t);
void framebuffer_draw_char(uint8_t, uint32_t, uint32_t);
uint64_t framebuffer_blit_image(uint32_t x, uint32_t y, char *xpm[]);
#endif
