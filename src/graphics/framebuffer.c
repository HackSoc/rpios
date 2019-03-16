#include <stdbool.h>
#include <stddef.h>
#include "../general_types.h"
#include "../integration/platform_specific.h"
#include "framebuffer.h"
#include "../gpu/mailbox.h"

static volatile uint32_t *internal_frame_buffer_pointer;
static bool internal_framebuffer_inited = false;

//Workaround for a horrible gcc bug
static struct FrameBufferConfig internal_framebuffer_config = {0};

ErrorStatus fb_init(uint32_t width, uint32_t height)
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
            internal_frame_buffer_pointer = (volatile uint32_t *)(internal_framebuffer_config.addr - 0xC0000000);

            return STATUS_OK;
        }
    }
    return STATUS_ERROR;
}

void fb_set_pixel(uint32_t x, uint32_t y, uint32_t colour)
{
    *(internal_frame_buffer_pointer + x + internal_framebuffer_config.width * y) = colour;
}

void fb_set_pixel_safe(uint32_t x, uint32_t y, uint32_t colour)
{
    if (x + internal_framebuffer_config.width * y < internal_framebuffer_config.width * internal_framebuffer_config.height)
    {
        fb_set_pixel(x, y, colour);
    }
}
