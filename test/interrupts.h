#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

void Reset_Handler (void);
void NonMaskableInt_Handler (void);
void HardFault_Handler (void);
void MemoryManagement_Handler (void);
void BusFault_Handler (void);
void UsageFault_Handler (void);
void DebugMonitor_Handler (void);
void TC0_CH0_InterruptHandler (void);
void TC0_CH1_InterruptHandler (void);
void TC0_CH2_InterruptHandler (void);
void SPW_InterruptHandler (void);

#endif // INTERRUPTS_H
