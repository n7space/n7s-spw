// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// @file spw.h
/// @brief Header for the n7s Spacewire (Spw) driver wrapper.

#ifndef N7S_SPW_H
#define N7S_SPW_H

#include "link.h"
#include "microchip_spw/plib_spw.h"
#include "rx.h"
#include "tx.h"

#define SPW_LINK_SELECT_COUNT 2

#ifdef __cplusplus
extern "C" {
#endif

/// @brief A typedef for a function pointer serving as a transmission end callback.
typedef void (*Spw_TxCallback)(void* arg, const Spw_Tx_IrqStatus* const irqStatus);

/// @brief A typedef for a function pointer serving as a reception callback.
typedef void (*Spw_RxCallback)(void* arg, const Spw_Rx_IrqStatus* const irqStatus);

/// @brief Structure serving as a transmission end callback descriptor.
typedef struct {
  /// @brief Callback function pointer.
  Spw_TxCallback callback;
  /// @brief Argument passed to the callback function.
  void* arg;
} Spw_TxHandler;

/// @brief Structure serving as a reception callback descriptor.
typedef struct {
  /// @brief Callback function pointer.
  Spw_RxCallback callback;
  /// @brief Argument passed to the callback function.
  void* arg;
} Spw_RxHandler;

/// @brief Structure representing a Spacewire instance.
typedef struct {
  /// @brief Spacewire Links instances.
  Spw_Link link[SPW_LINK_SELECT_COUNT];
  /// @brief Spacewire PktRx instance.
  Spw_Rx rx;
  /// @brief Spacewire PktTx instance.
  Spw_Tx tx;
  /// @brief Registered transmission end callback.
  Spw_TxHandler txHandler;
  /// @brief Registered reception callback.
  Spw_RxHandler rxHandler;
} Spw;

/// @brief Structure representing a configuration descriptor for Spacewire.
typedef struct {
  /// @brief Spw links configuration.
  Spw_Link_Config link[SPW_LINK_SELECT_COUNT];
  /// @brief PktRx configuration.
  Spw_Rx_Config rx;
  /// @brief PktTx configuration.
  Spw_Tx_Config tx;
} Spw_Config;

/// @brief Initializes Spacewire instance.
/// @param[out] spw    Spacewire instance.
void Spw_init(Spw* const spw);

/// @brief Sets Spacewire config.
/// @param[out] spw     Spacewire instance.
/// @param[in] config  Spacewire configuration.
void Spw_setConfig(Spw* const spw, const Spw_Config* const config);

/// @brief Function used to register a transmission end callback in the driver.
/// @param[out] spw     Spacewire instance.
/// @param[in] handler  Transmission end callback handler.
void Spw_registerTxHandler(Spw* const spw, const Spw_TxHandler handler);

/// @brief Function used to register a reception callback in the driver.
/// @param[out] spw     Spacewire instance.
/// @param[in] handler  Reception callback handler.
void Spw_registerRxHandler(Spw* const spw, const Spw_RxHandler handler);

/// @brief Function used to perform driver-specific operations when handling a
/// SpaceWire interrupt
///        from outside of the driver.
/// @param[out] spw     Spacewire instance.
void Spw_handleInterrupt(Spw* const spw);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // N7S_SPW_H
