#include "mailbox.h"

uint32_t mbox_read(uint8_t chan)
{
    uint32_t data;
    while (1)
    {
        while (*MBOX_R_S & 0x40000000)
            ;
        data = *MBOX_R_D;
        if ((data & 0xF) == chan)
            break;
    }
    return data & 0xfffffff0;
}

void mbox_write(uint32_t data, uint8_t chan)
{
    while (*MBOX_W_S & 0x80000000)
        ;
    *MBOX_W_D = data + chan;
}
