#ifndef RPIOS_FONT_H
#define RPIOS_FONT_H
#pragma once

#include <stdint.h>

/**
 * Font characters are 16 rows tall and 8 columns wide. Hence each bit in the
 * uint8_t represent a single pixel of the final character, 1 is white, 0 is
 * black.
 */
typedef struct
{
    uint8_t rows[16];
} FontChar;

/**
 * Fonts are made up of 256 characters, each character is 16 bytes in size.
 * This means that a full font is 4096 bytes and an ASCII only font is 2048.
 *
 * It goes without saying that using an ASCII only font can lead to memory
 * safety issues.
 */
typedef struct
{
    FontChar chars[256];
} Font;

void font_draw_char(Font *font, uint8_t char_index, uint8_t pos_x, uint8_t pos_y);

extern Font font_default;

#endif
