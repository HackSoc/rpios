SECTIONS {
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }

    .init 0x8000 : {
        *(.init)
    }

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
        *(.bss)
        *(.rodata*)
        *(.COMMON)
    }

    /DISCARD/ : {
        *(*)
    }
}
