/*
 * Linker script for ATSAMRH71F20C – arm-none-eabi-gcc
 *
 *   rom         (flash HEFC) : 0x10000000 / 128 KB
 *   ram         (FlexRAM)    : 0x21000000 / 512 KB
 *   ram_nocache              : 0x21080000 / 256 KB  non-cached alias, use for SPW DMA buffers
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
  rom         (LRX)  : ORIGIN = 0x10000000, LENGTH = 0x00020000
  ram         (WX!R) : ORIGIN = 0x21000000, LENGTH = 0x00080000
  ram_nocache (WX!R) : ORIGIN = 0x21080000, LENGTH = 0x00040000
}

_stack = ORIGIN(ram) + LENGTH(ram);

SECTIONS
{
  /* ---- vector table ---------------------------------------------------- */
  .vectors :
  {
    __svectors = .;
    KEEP(*(.vectors .vectors.*))
    KEEP(*(.isr_vector))
  } > rom

  /* ---- code and read-only data ----------------------------------------- */
  .text :
  {
    . = ALIGN(4);
    *(.text .text.*)
    *(.rodata .rodata.*)
    *(.ARM.extab* .gnu.linkonce.armextab.*)

    /* constructor / destructor tables for __libc_init_array */
    KEEP(*(.init))
    __preinit_array_start = .;
    KEEP(*(.preinit_array))
    __preinit_array_end   = .;
    __init_array_start = .;
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    __init_array_end   = .;
    KEEP(*(.fini))
    __fini_array_start = .;
    KEEP(*(.fini_array))
    KEEP(*(SORT(.fini_array.*)))
    __fini_array_end   = .;
    . = ALIGN(4);
  } > rom

  .ARM.exidx :
  {
    __exidx_start = .;
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    __exidx_end   = .;
  } > rom

  . = ALIGN(4);
  _etext = .;   /* LMA of initialised data */

  /* ---- non-cached section (VMA in ram_nocache, loads from rom) ----------
   * Place SPW DMA buffers here with __attribute__((section(".ram_nocache")))
   * ----------------------------------------------------------------------- */
  .ram_nocache (NOLOAD) :
  {
    . = ALIGN(4);
    *(.ram_nocache .ram_nocache.*)
    . = ALIGN(4);
  } > ram_nocache

  /* ---- initialised data (LMA in rom, VMA in ram) ----------------------- */
  .data : AT(_etext)
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data .data.*)
    . = ALIGN(4);
    _edata = .;
  } > ram

  _sidata = LOADADDR(.data);

  /* ---- zero-initialised data ------------------------------------------- */
  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss .bss.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > ram

  . = ALIGN(4);
  _end = .;
  PROVIDE(end = _end);

  /DISCARD/ : { *(.note.GNU-stack) }
}
