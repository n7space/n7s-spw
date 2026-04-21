/*
 * Linker script for ATSAMRH71F20C
 *
 *   ram         (FlexRAM)    : 0x21000000 / 512 KB
 *   ram_nocache              : 0x21080000 / 256 KB
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
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
  } > ram

  /* ---- code and read-only data ----------------------------------------- */
  .text :
  {
    . = ALIGN(4);
    *(.text .text.*)
    *(.rodata .rodata.*)
    . = ALIGN(4);
  } > ram

  /* ---- initialised data ------------------------------------------------ */
  .data :
  {
    . = ALIGN(4);
    *(.data .data.*)
    . = ALIGN(4);
  } > ram

  /* ---- non-cached section ---------------------------------------------- */
  .ram_nocache (NOLOAD) :
  {
    . = ALIGN(4);
    *(.ram_nocache .ram_nocache.*)
    . = ALIGN(4);
  } > ram_nocache

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

  /DISCARD/ : { *(.note.GNU-stack) *(.ARM.exidx*) *(.ARM.extab*) *(.init) *(.fini) *(.preinit_array) *(.init_array*) *(.fini_array*) }
}
