#include "mailbox.h"
#include "../memory/mem.h"

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

uint8_t* mbox_prop_get(uint32_t channel, uint32_t tag, uint32_t request_bytes, uint8_t* request, uint32_t response_bytes) {
    // Mailbox properties allow making various calls and requests to the
    // hardware. 
    // They're quite well documented here:
    // https://github.com/raspberrypi/documentation/blob/JamesH65-mailbox_docs/configuration/mailboxes/propertiesARM-VC.md
    // Bascially:
    //   - Create a buffer like this:
    //     +---- u32 -----+---- u32 -----+-- variable --+-- u32 --+
    //     | Buffer size  | Request code |     Tag...   |   0x0   |
    //     |              |   (always 0) |              |         |
    //     +--------------+--------------+--------------+---------+
    //     
    //   - The aforementioned tag takes this format:
    //     +---- u32 -----+---- u32 -----+---- u32 -----+-- variable --+
    //     |    Tag ID    | Value buffer | Request code |     Value    |
    //     |              |     size     |  (msb = 0)   |   buffer...  |
    //     +--------------+--------------+--------------+--------------+
    //     (The value buffer must be large enough to store the largest of the
    //     request or response of the tag's property.)
    //
    //   - Put the address of the buffer into a mailbox, usually with channel 8.
    //     (mbox_write((uint32_t)buffer, 8))
    //
    //   - Read the data out of the mailbox and interpret it as a pointer; it'll
    //     probably be the same as the address of the original buffer but there
    //     seems to be no guarantee of this.
    //     ((uint32_t*)mbox_read(8))
    //
    //   - The value buffer will contain the response. This can be more granular
    //     than u32 so this function casts it down to u8.

    // Clear the buffer first
    memset(mbox_prop_buffer, 0, sizeof(uint32_t) * PROP_BUFFER_SIZE);

    // Total buffer size
    mbox_prop_buffer[0] = sizeof(uint32_t) * PROP_BUFFER_SIZE;

    // Request code, always 0
    mbox_prop_buffer[1] = 0;

    // Tag ID 
    mbox_prop_buffer[2] = tag;

    // Value buffer size
    mbox_prop_buffer[3] =
        request_bytes > response_bytes
            ? request_bytes
            : response_bytes;

    // Request code
    mbox_prop_buffer[4] = 0;

    // Do a weird memcpy into the tag value buffer
    for (uint32_t i = 0; i < request_bytes; i++) {
        ((uint8_t*)(mbox_prop_buffer[5]))[i] = request[i];
    }

    // Write to the mailbox and read the result
    mbox_write((uint32_t)mbox_prop_buffer, 8);
    uint32_t* response = (uint32_t*)mbox_read(8); 

    // Return a pointer to the value buffer
    return (uint8_t*)(response + 5);
}
