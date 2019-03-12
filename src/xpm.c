#include "xpm.h"
#include "framebuffer.h"

uint64_t xpm_blit_image(uint32_t x, uint32_t y, char *xpm[])
{
    int i = 0;
    int width = 0;
    for (; xpm[0][i] != ' '; i++)
    {
        width = width * 10 + xpm[0][i] - '0';
    }
    i++;
    int height = 0;
    for (; xpm[0][i] != ' '; i++)
    {
        height = height * 10 + xpm[0][i] - '0';
    }
    i++;
    int colors = 0;
    for (; xpm[0][i] != ' '; i++)
    {
        colors = colors * 10 + xpm[0][i] - '0';
    }
    i++;
    int chars = 0;
    for (; xpm[0][i] != '\0'; i++)
    {
        chars = chars * 10 + xpm[0][i] - '0';
    }

    struct Color
    {
        char ch;
        uint32_t co;
        bool transparent;
    };
    struct Color colortable[colors]; // VLAs are great, what are you talking about

    for (int i = 0; i < colors; i++)
    {
        if ((xpm[i + 1][4] == 'N') || (xpm[i + 1][4] == 'n'))
        {
            // transparent pixel, rest of the word is "none"
            colortable[i] = (struct Color){.ch = xpm[i + 1][0], .co = 0, .transparent = true};
        }
        else
        {
            uint32_t color = 0;
            for (int j = 0; j < 6; j++)
            {
                char c = xpm[i + 1][j + 5];
                color = color * 16 + (('A' <= c) && (c <= 'F') ? c - ('A' - 10) : c - '0');
            }
            colortable[i] = (struct Color){.ch = xpm[i + 1][0], .co = color, .transparent = false};
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            uint8_t k;
            for (k = 0; k < colors; k++)
            {
                if (xpm[i + colors + 1][j] == colortable[k].ch)
                {
                    break;
                }
            }
            if (!colortable[k].transparent)
            {
                framebuffer_set_pixel(j + x, i + y, colortable[k].co);
            }
        }
    }

    return ((uint64_t)height << 32) | width;
}
