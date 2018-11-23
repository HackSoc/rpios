#include <stddef.h>
#include "framebuffer.h"
#include "mailbox.h"

static volatile uint32_t *fb;
//Workaround for a horrible gcc bug
static struct fbcfg fbcfg = {0};

volatile uint32_t *fb_init(uint32_t width, uint32_t height)
{
    fbcfg.width = width;
    fbcfg.height = height;
    fbcfg.vwidth = width;
    fbcfg.vheight = height;
    fbcfg.depth = 32;
    mbox_write(((uint32_t) &fbcfg)+0xC0000000, 1);
    if(!mbox_read(1)){
        fb = (volatile uint32_t *) (fbcfg.addr-0xC0000000);
        return fb;
    }
    return NULL;
}

void setPixel(uint32_t x, uint32_t y, uint32_t colour)
{
    *(fb+x+fbcfg.width*y) = colour;
}
