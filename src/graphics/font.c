#include "font.h"
#include "framebuffer.h"

void font_draw_char(Font *font, uint8_t char_index, uint8_t pos_x, uint8_t pos_y)
{
    FontChar character = font->chars[char_index];
    for (uint8_t row_index = 0; row_index < 16; row_index++)
    {
        uint8_t row = character.rows[row_index];
        for (uint8_t column_index = 0; column_index < 8; column_index++)
        {
            fb_set_pixel(pos_x + column_index, pos_y + row_index, row & (1 << column_index) ? 0xFFFFFFFF : 0x0);
        }
    }
}

void font_draw_char_colour(Font *font, uint8_t char_index, uint8_t pos_x, uint8_t pos_y, uint32_t colour)
{
    FontChar character = font->chars[char_index];
    for (uint8_t row_index = 0; row_index < 16; row_index++)
    {
        uint8_t row = character.rows[row_index];
        for (uint8_t column_index = 0; column_index < 8; column_index++)
        {
            fb_set_pixel_safe(pos_x + column_index, pos_y + row_index, row & (1 << column_index) ? colour : 0x0);
        }
    }
}
