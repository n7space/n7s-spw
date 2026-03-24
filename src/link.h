// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file link.h
/// \brief Header for the n7s Spacewire (Spw) link driver wrapper.

#ifndef N7S_SPW_LINK_H
#define N7S_SPW_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/// \brief Initializes a device descriptor for Spacewire link.
void Spw_Link_init();

/// \brief Sets Spacewire Link config.
void Spw_Link_setConfig();

/// \brief Reads Spacewire Link config.
void Spw_Link_getConfig();

/// \brief Resets Spacewire Link to default state.
void Spw_Link_reset();

/// \brief Reads Spacewire Link status.
void Spw_Link_getStatus();

/// \brief Sets Spacewire Link interrupt mask.
void Spw_Link_setIrqMask();

/// \brief Reads Spacewire Link interrupts.
uint32_t Spw_Link_getIrq();

/// \brief Reads Spacewire Link masked interrupts.
uint32_t Spw_Link_getIrqMasked();

/// \brief Reads and clears Spacewire Link interrupts.
/// \returns Interrupt status register.
uint32_t Spw_Link_getAndClearIrq();

/// \brief Reads and clears masked Spacewire Link interrupts.
/// \returns Masked interrupt status register.
uint32_t Spw_Link_getAndClearIrqMasked();

/// \brief Clears Spacewire Link interrupts.
void Spw_Link_clearIrq();

/// \brief Decodes Link interrupt status register to a structure.
void Spw_Link_getStructuredIrq();

/// \brief Sets Distributed Interrupt irq mask.
void Spw_Link_setDistributedInterruptIrqMask();

/// \brief Gets Distributed Interrupt irq.
uint32_t Spw_Link_getDistributedInterruptIrq();

/// \brief Gets Distributed Interrupt masked irq.
uint32_t Spw_Link_getDistributedInterruptIrqMasked();

/// \brief Gets and clears Distributed Interrupt irq.
uint32_t Spw_Link_getAndClearDistributedInterruptIrq();

/// \brief Gets and clears Distributed Interrupt masked irq.
uint32_t Spw_Link_getAndClearDistributedInterruptIrqMasked();

/// \brief Clears Distributed Interrupt irq.
void Spw_Link_clearDistributedInterruptIrq();

/// \brief Gets Distributed Interrupt Acknowledge irq.
uint32_t Spw_Link_getDistributedInterruptAckIrq();

/// \brief Gets Distributed Interrupt Acknowledge masked irq.
uint32_t Spw_Link_getDistributedInterruptAckIrqMasked();

/// \brief Gets and clears Distributed Interrupt Acknowledge irq.
uint32_t Spw_Link_getAndClearDistributedInterruptAckIrq();

/// \brief Gets and clears Distributed Interrupt Acknowledge masked irq.
uint32_t Spw_Link_getAndClearDistributedInterruptAckIrqMasked();

/// \brief Clears Distributed Interrupt Acknowledge irq.
void Spw_Link_clearDistributedInterruptAckIrq();

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_LINK_H