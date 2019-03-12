#include <stdbool.h>
#include <stddef.h>
#include "general_types.h"
#include "platform_specific.h"
#include "framebuffer.h"
#include "mailbox.h"

static volatile address_t *internal_frame_buffer_pointer;
static bool internal_framebuffer_inited = false;

//Workaround for a horrible gcc bug
static struct FrameBufferConfig internal_framebuffer_config = {0};

ErrorStatus framebuffer_init(uint32_t width, uint32_t height)
{
    if (!internal_framebuffer_inited)
    {
        internal_framebuffer_inited = true;

        internal_framebuffer_config.width = width;
        internal_framebuffer_config.height = height;
        internal_framebuffer_config.vwidth = width;
        internal_framebuffer_config.vheight = height;
        internal_framebuffer_config.depth = 32;

        mbox_write(((uint32_t)&internal_framebuffer_config) + 0xC0000000, 1);

        if (!mbox_read(1))
        {
            internal_frame_buffer_pointer = (volatile address_t *)(internal_framebuffer_config.addr - 0xC0000000);

            return STATUS_OK;
        }
    }
    return STATUS_ERROR;
}

void framebuffer_set_pixel(uint32_t x, uint32_t y, uint32_t colour)
{
    *(internal_frame_buffer_pointer + x + internal_framebuffer_config.width * y) = colour;
}

void framebuffer_set_pixel_safe(uint32_t x, uint32_t y, uint32_t colour)
{
    if (x + internal_framebuffer_config.width * y < internal_framebuffer_config.width * internal_framebuffer_config.height)
    {
        framebuffer_set_pixel(uint32_t x, uint32_t y, uint32_t colour);
    }
}

void framebuffer_draw_char(uint8_t chr, uint32_t x, uint32_t y, uint8_t *font)
{
    for (uint32_t py = 0; py < 16; py++)
    {
        uint8_t row = *(font + chr * 16 + py);
        for (uint32_t px = 0; px < 8; px++)
        {
            framebuffer_set_pixel(x + px, y + py, row & (1 << px) ? 0xFFFFFFFF : 0x0);
        }
    }
}
