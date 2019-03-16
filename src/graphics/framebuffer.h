#ifndef RPIOS_FRAMEBUFFER_H
#define RPIOS_FRAMEBUFFER_H
#pragma once

#include <stdint.h>

#include "../integration/compiler_specific.h"
#include "../general_types.h"

typedef struct FrameBufferConfig
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
} FrameBufferConfig __attribute__((aligned(16)));

ErrorStatus fb_init(uint32_t width, uint32_t height);
void fb_set_pixel(uint32_t, uint32_t, uint32_t);
void fb_set_pixel_safe(uint32_t x, uint32_t y, uint32_t colour);

#endif
