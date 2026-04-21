#include <stdbool.h>
#include <stdint.h>

#include <Nvic/Nvic.h>

#include "device.h"

extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t __svectors;

extern int main(void);

void __attribute__((noreturn, section(".text.Reset_Handler"))) Reset_Handler(void)
{
  // 1. Zero .bss
  {
    uint32_t* dst = &_sbss;
    const uint32_t* end = &_ebss;
    while (dst < end)
    {
      *dst++ = 0u;
    }
  }

  // 2. Relocate vector table
  Nvic_relocateVectorTable(&__svectors);

  // 3. Run application
  (void)main();

  while (true)
  {
  }
}
