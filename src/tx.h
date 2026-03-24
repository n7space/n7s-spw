// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file tx.h
/// \brief Header for the n7s Spacewire (Spw) Tx driver wrapper.

#ifndef N7S_SPW_TX_H
#define N7S_SPW_TX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/// \brief Initializes a device descriptor for Spacewire tx.
void Spw_Tx_init();

/// \brief Sets Spacewire PktTx config.
void Spw_Tx_setConfig();

/// \brief Resets Spacewire PktTx to default state.
void Spw_Tx_reset();

/// \brief Sets next Send List.
void Spw_Tx_setNextSendList();

/// \brief Sets next Send List entry.
void Spw_Tx_setNextSendListEntry();

/// \brief Aborts transmission of current Send List.
void Spw_Tx_abortCurrentSendList();

/// \brief Gets current Send List Config.
void Spw_Tx_getCurrentSendListConfig();

/// \brief Gets Send List entry structure.
void Spw_Tx_getSendListEntry();

/// \brief Sets PktTx interrupt mask.
void Spw_Tx_setIrqMask();

/// \brief Gets PktTx interrupts.
uint32_t Spw_Tx_getIrq();

/// \brief Gets masked PktTx interrupts.
uint32_t Spw_Tx_getIrqMasked();

/// \brief Gets and clears PktTx interrupts.
uint32_t Spw_Tx_getAndClearIrq();

/// \brief Gets and clears masked PktTx interrupts.
uint32_t Spw_Tx_getAndClearIrqMasked();

/// \brief Clears PktTx interrupts.
void Spw_Tx_clearIrq();

/// \brief Decodes PktTx interrupt status register to a structure.
void Spw_Tx_getStructuredIrq();

/// \brief Gets PktTx status register.
void Spw_Tx_getStatus();

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_TX_H