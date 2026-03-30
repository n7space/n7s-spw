// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file rx.h
/// \brief Header for the n7s Spacewire (Spw) RX driver wrapper.

#ifndef N7S_SPW_RX_H
#define N7S_SPW_RX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/// \brief Initializes a device descriptor for Spacewire rx.
void Spw_Rx_init();

/// \brief Sets Spacewire PktRx config.
void Spw_Rx_setConfig();

/// \brief Resets Spacewire PktRx to default state.
void Spw_Rx_reset();

/// \brief Gets PktRx interrupts.
uint32_t Spw_Rx_getIrq();

/// \brief Gets masked PktRx interrupts.
uint32_t Spw_Rx_getIrqMasked();

/// \brief Gets and clears PktRx interrupts.
uint32_t Spw_Rx_getAndClearIrq();

/// \brief Gets and clears masked PktRx interrupts.
uint32_t Spw_Rx_getAndClearIrqMasked();

void Spw_Rx_clearIrq();

/// \brief Sets next Receive Buffer.
void Spw_Rx_setNextRxBuffer();

/// \brief Aborts current Receive Buffer.
void Spw_Rx_abortOngoingPacketRx();

/// \brief Gets next Receive Buffer config.
void Spw_Rx_getNextRxBufferConfig();

/// \brief Gets PktRx status.
void Spw_Rx_getStatus();

/// \brief Gets previous Receive Buffer status.
void Spw_Rx_getPreviousRxBufferStatus();

/// \brief Decodes Receive Buffer entry.
void Spw_Rx_getRxBufferEntry();

/// \brief Decodes PktRx interrupt register.
void Spw_Rx_getStructuredIrq();

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_RX_H