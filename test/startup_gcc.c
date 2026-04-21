#include <stdbool.h>
#include <stdint.h>

#include <Nvic/Nvic.h>
#include <Scb/Scb.h>

#include "device.h"

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t __svectors;
extern uint32_t _stack;
extern uint32_t _stack_nocache;

extern int main(void);

// FPU coprocessor access register
#define SCB_CPACR_REG (*(volatile uint32_t*)0xE000ED88u)

// ECC seed-write chunk size = 16 VFP double regs × 8 B = 128 B
#define ECC_CHUNK_SIZE 0x80u

// FPU enable – must happen before any VLDM/VSTM (used in ECC init).
static void fpu_enable(void)
{
  SCB_CPACR_REG |= (0xFu << 20u);
  asm volatile("dsb" ::: "memory");
  asm volatile("isb" ::: "memory");
}

// FlexRAM ECC seed writes.
static void __attribute__((noinline)) ecc_read_chunk(uint32_t addr)
{
  asm volatile("MOV  r8, %[a]\n"
               "PLD  [r8, #0xC0]\n"
               "VLDM r8, {d0-d15}\n"
    :
    : [a] "r"(addr)
    : "r8", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
      "d8", "d9", "d10", "d11", "d12", "d13", "d14", "d15", "memory");
}

static void __attribute__((noinline)) ecc_write_chunk(uint32_t addr)
{
  asm volatile("MOV  r8, %[a]\n"
               "VSTM r8, {d0-d15}\n"
    :
    : [a] "r"(addr)
    : "r8", "memory");
}

static void __attribute__((optimize("-O1"))) flexram_ecc_initialize(void)
{
  asm volatile("dsb" ::: "memory");
  asm volatile("isb" ::: "memory");
  for (uint32_t addr = FLEXRAM_ADDR; addr < (FLEXRAM_ADDR + FlexRAM_SIZE); addr += ECC_CHUNK_SIZE)
  {
    ecc_read_chunk(addr);
    asm volatile("dsb" ::: "memory");
    asm volatile("isb" ::: "memory");
    ecc_write_chunk(addr);
    asm volatile("dsb" ::: "memory");
    asm volatile("isb" ::: "memory");
  }
  asm volatile("dsb" ::: "memory");
  asm volatile("isb" ::: "memory");
}

void __attribute__((noreturn, section(".text.Reset_Handler"))) Reset_Handler(void)
{
  // 1. Enable FPU before any VLDM/VSTM
  fpu_enable();

  // 2. Seed FlexRAM ECC (must come before .bss zero / stack use).
  //    The loop overwrites all of FlexRAM including the current stack area,
  //    so temporarily switch MSP to the ECC-free ram_nocache region.
  asm volatile("MSR msp, %0" :: "r"(&_stack_nocache) : "memory");
  flexram_ecc_initialize();
  asm volatile("MSR msp, %0" :: "r"(&_stack) : "memory");

  // 3. Copy .data from flash to RAM
  {
    uint32_t* src = &_sidata;
    uint32_t* dst = &_sdata;
    const uint32_t* end = &_edata;
    while (dst < end)
    {
      *dst++ = *src++;
    }
  }

  // 4. Zero .bss
  {
    uint32_t* dst = &_sbss;
    const uint32_t* end = &_ebss;
    while (dst < end)
    {
      *dst++ = 0u;
    }
  }

  // 5. Relocate vector table
  Nvic_relocateVectorTable(&__svectors);

  // 6. Run application
  (void)main();

  while (true)
  {
  }
}
