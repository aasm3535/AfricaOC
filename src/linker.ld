/* src/linker.ld */
ENTRY(_start)
OUTPUT_FORMAT("binary")

SECTIONS {
    /* Адрес, по которому загрузчик загружает ядро */
    . = 0x7E00;

    .text : {
        *(.text)
    }
    .rodata : {
        *(.rodata)
    }
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }
    /DISCARD/ : {
        *(.eh_frame)
        *(.comment)
    }
}
