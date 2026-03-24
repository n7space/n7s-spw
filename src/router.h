// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file router.h
/// \brief Header for the n7s Spacewire (Spw) Router driver wrapper.

#ifndef N7S_SPW_ROUTER_H
#define N7S_SPW_ROUTER_H

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Initializes a device descriptor for Spacewire Router.
void Spw_Router_init();

/// \brief Sets Spacewire Router config.
void Spw_Router_setConfig();

/// \brief Gets Spacewire Router status.
void Spw_Router_getStatus();

/// \brief Gets Router table entry.
void Spw_Router_getTableEntry();

/// \brief Sets Router table entry.
void Spw_Router_setTableEntry();

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_ROUTER_H