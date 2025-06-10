/* src/linker.ld */
ENTRY(_start)
OUTPUT_FORMAT("binary")

SECTIONS {
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

    /* Эта строка создает символ 'end', который указывает на конец образа ядра.
       С этого адреса начнется наша куча (heap). */
    end = .;

    /DISCARD/ : {
        *(.eh_frame)
        *(.comment)
    }
}
