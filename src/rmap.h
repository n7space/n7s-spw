// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file rmap.h
/// \brief Header for the n7s Spacewire (Spw) RMAP driver wrapper.

#ifndef N7S_SPW_RMAP_H
#define N7S_SPW_RMAP_H

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Initializes a device descriptor for Spacewire rmap.
void Spw_Rmap_init();

/// \brief Sets Spacewire Rmap config.
void Spw_Rmap_setConfig();

/// \brief Gets Spacewire Rmap status.
void Spw_Rmap_getStatus();

/// \brief Gets and clears Spacewire Rmap status.
void Spw_Rmap_getAndClearStatus();

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_RMAP_H