
MEMORY
{
    FLASH  (rx): ORIGIN = 0x08000000, LENGTH = 16K
    RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 4K
}

ENTRY(Reset_Handler)
SECTIONS
{
   . = ORIGIN(FLASH);
    .text : {
        KEEP(*(.stack))
        KEEP(*(.vectors))
        KEEP(*(.vectors*))
        KEEP(*(.text))
        . = ALIGN(4);
        *(.text*)
        . = ALIGN(4);
        KEEP(*(.rodata))
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    .init_array ALIGN(4): {
        __init_array_start = .;
        KEEP(*(.init_array))
        __init_array_end = .;
    } >FLASH

    __stacktop = ORIGIN(RAM) + LENGTH(RAM);
    __data_load = LOADADDR(.data);
    . = ORIGIN(RAM);

    .data ALIGN(4) : {
        __data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end = .;
    } >RAM AT >FLASH

    .bss ALIGN(4) (NOLOAD) : {
        __bss_start__ = .;
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
        __bss_end__ = .;
        *(.noinit)
        *(.noinit*)
    } >RAM

    . = ALIGN(4);
    __heap_start__ = .;
    end = __heap_start__;
}