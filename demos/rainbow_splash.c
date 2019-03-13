#include "rainbow_splash.h"
#include "../framebuffer.h"

void rainbow_splash(uint8_t b) {
    uint8_t r, g;
    for(r=0; r < 0xff; r++) {
        for(g=0; g < 0xff; g++) {
            setPixel(2*r  ,2*g  ,(r << 16) | (g << 8) | b);
            setPixel(2*r+1,2*g  ,(r << 16) | (g << 8) | b);
            setPixel(2*r  ,2*g+1,(r << 16) | (g << 8) | b);
            setPixel(2*r+1,2*g+1,(r << 16) | (g << 8) | b);
        }
    }

    setPixel(0x210, (b+0)*2, 0x00);
    setPixel(0x210, (b+1)*2, 0xffffff);
    setPixel(0x210, (b+2)*2, 0x00);
}