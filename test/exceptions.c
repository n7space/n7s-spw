#include "definitions.h"
#include "interrupts.h"

void __attribute__((noreturn, weak)) NonMaskableInt_Handler(void)
{
  while (true)
  {
  }
}

void __attribute__((noreturn, weak)) HardFault_Handler(void)
{
  while (true)
  {
  }
}

void __attribute__((noreturn, weak)) DebugMonitor_Handler(void)
{
  while (true)
  {
  }
}

void __attribute__((noreturn, weak)) MemoryManagement_Handler(void)
{
  while (true)
  {
  }
}

void __attribute__((noreturn, weak)) BusFault_Handler(void)
{
  while (true)
  {
  }
}

void __attribute__((noreturn, weak)) UsageFault_Handler(void)
{
  while (true)
  {
  }
}
