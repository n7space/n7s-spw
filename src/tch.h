// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file tch.h
/// \brief Header for the n7s Spacewire (Spw) Tch driver wrapper.

#ifndef N7S_SPW_TCH_H
#define N7S_SPW_TCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/// \brief Initializes a device descriptor for Spacewire tch.
void Spw_Tch_init();

/// \brief Sets Tch config.
void Spw_Tch_setConfig();

/// \brief Resets Spacewire Tch to the default state.
void Spw_Tch_reset();

/// \brief Sends Tch Time Code.
void Spw_Tch_sendTimeCode();

/// \brief Gets last Time Code.
uint8_t Spw_Tch_getLastTimeCode();

/// \brief Gets Tch interrupts.
uint32_t Spw_Tch_getIrq();

/// \brief Gets masked Tch interrupts.
uint32_t Spw_Tch_getIrqMasked();

/// \brief Gets and clears Tch interrupts.
uint32_t Spw_Tch_getAndClearIrq();

/// \brief Gets and clears masked Tch interrupts.
uint32_t Spw_Tch_getAndClearIrqMasked();

/// \brief Clears Tch interrupts.
void Spw_Tch_clearIrq();

/// \brief Decodes Tch interrupt register.
void Spw_Tch_getStructuredIrq();

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_TCH_H