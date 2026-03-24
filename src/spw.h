// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file spw.h
/// \brief Header for the n7s Spacewire (Spw) driver wrapper.

#ifndef N7S_SPW_H
#define N7S_SPW_H

#include "microchip_spw/plib_spw.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Initializes Spacewire instance.
void Spw_init();

/// \brief Sets Spacewire config.
void Spw_setConfig();

/// \brief Function used to register a transmission end callback in the driver.
void Spw_registerCallback(SPW_CALLBACK callback, uintptr_t contextHandle);

/// \brief Function used to register a reception callback in the driver.
void Spw_registerRxHandler(SPW_CALLBACK callback, uintptr_t contextHandle);

/// \brief Function used to register a time channel callback in the driver.
void Spw_registerTchHandler(SPW_CALLBACK callback, uintptr_t contextHandle);

/// \brief Function used to perform driver-specific operations when handling a SpaceWire interrupt
///        from outside of the driver.
void Spw_handleInterrupt();

#ifdef __cplusplus
} // extern "C"
#endif


#endif // N7S_SPW_H