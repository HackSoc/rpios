/*
 * The raspberry pi consists of a graphical processing unit (GPU) and central processing unit (CPU) on the same chip
 * These chips communicate through the use of shared memory in a "mailbox" system
 *
 * This program runs on the CPU and communicates with the GPU to set up a "framebuffer":
 *     A framebuffer is a location in memory to which pixel data can be written to have it appear on the screen
 *     We create this framebuffer by telling the GPU the address of a structure containing the resolution and bits per colour of the required buffer
 *     The GPU then answers back with the address of the buffer we can write to
 */

.section .init
.global _start
_start:
    .extern kmain
    mov sp,#0x8000
    bl kmain

.section .data
.global font
font:
    .incbin "font.bin"
