// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// @file router.h
/// @brief Header for the n7s Spacewire (Spw) router driver wrapper.

#ifndef N7S_SPW_ROUTER_H
#define N7S_SPW_ROUTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

/// @brief Spacewire Router configuration.
typedef struct {
  /// @brief Disable connectivity to physical address timeout.
  bool isTimeoutDisabled;
  /// @brief Enable fallback routing.
  bool isFallbackRoutingEnabled;
  /// @brief Enable logical address routing.
  bool isLogicalAddressRoutingEnabled;
} Spw_Router_Config;

/// @brief Spacewire Router status.
typedef struct {
  /// @brief Indicates number of dropped packets. Saturates at 255.
  uint8_t droppedPacketCount;
  /// @brief Router byte of first dropped packet.
  uint8_t routerByteOfFirstDroppedPacket;
  /// @brief Source of first dropped packet.
  uint8_t sourceOfFirstDroppedPacket;
  /// @brief Destination from table of first dropped packet.
  uint8_t destinationFromTableOfFirstDroppedPacket;
  /// @brief Did timeout occur.
  bool timeoutOccurred;
  /// @brief Identifies destination that stalled.
  uint8_t timeoutDestinationAddress;
} Spw_Router_Status;

/// @brief Spacewire Router logical address table entry configuration.
typedef struct {
  /// @brief Physical port address to route to.
  uint8_t address;
  /// @brief Discard router byte for this logical address.
  bool deleteHeaderByte;
} Spw_Router_TableEntry;

/// @brief Initializes the Spacewire Router.
void Spw_Router_init(void);

/// @brief Sets Spacewire Router configuration.
/// @param [in] config Spacewire Router configuration.
void Spw_Router_setConfig(const Spw_Router_Config* const config);

/// @brief Gets Spacewire Router status.
/// @param [out] status Router status structure.
void Spw_Router_getStatus(Spw_Router_Status* const status);

/// @brief Gets a routing table entry for a logical address.
/// @param [in] id Logical address table entry (32–255).
/// @param [out] entry Router table entry.
void Spw_Router_getTableEntry(const uint8_t id, Spw_Router_TableEntry* const entry);

/// @brief Sets a routing table entry for a logical address.
/// @param [in] id Logical address table entry (32–255).
/// @param [in] entry Router table entry.
void Spw_Router_setTableEntry(const uint8_t id, const Spw_Router_TableEntry* const entry);

#ifdef __cplusplus
}
#endif

#endif /* N7S_SPW_ROUTER_H */
