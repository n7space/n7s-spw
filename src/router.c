// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

#include "router.h"

#include <assert.h>
#include "device.h"
#include "microchip_spw/plib_spw_router.h"

void Spw_Router_init(void)
{
  SPW_ROUTER_Initialize();
}

void Spw_Router_setConfig(const Spw_Router_Config* const config)
{
  SPW_ROUTER_TimeoutDisable(config->isTimeoutDisabled);
  SPW_ROUTER_LogicalAddressRoutingEnable(config->isLogicalAddressRoutingEnabled);
  SPW_ROUTER_FallbackEnable(config->isFallbackRoutingEnabled);
}

void Spw_Router_getStatus(Spw_Router_Status* const status)
{
  const SPW_ROUTER_STATUS sts = SPW_ROUTER_StatusGet();
  status->droppedPacketCount = (uint8_t)SPW_ROUTER_STATUS_GET_COUNT(sts);
  status->routerByteOfFirstDroppedPacket = (uint8_t)SPW_ROUTER_STATUS_GET_BYTE(sts);
  status->sourceOfFirstDroppedPacket = (uint8_t)SPW_ROUTER_STATUS_GET_SOURCE(sts);
  status->destinationFromTableOfFirstDroppedPacket = (uint8_t)SPW_ROUTER_STATUS_GET_DEST(sts);

  const SPW_ROUTER_TIMEOUT_STATUS timeout = SPW_ROUTER_TimeoutStatusGet();
  status->timeoutOccurred = (timeout & SPW_ROUTER_TIMEOUT_STATUS_LOCKED) != 0U;
  status->timeoutDestinationAddress = (uint8_t)SPW_ROUTER_TIMEOUT_STATUS_GET_ADDR(timeout);
}

void Spw_Router_getTableEntry(const uint8_t id, Spw_Router_TableEntry* const entry)
{
  assert(id >= 32U);

  const uint32_t raw = SPW_REGS->SPW_ROUTER_TABLE[id - 32U];
  entry->address = (uint8_t)(raw & SPW_ROUTER_TABLE_ADDR_Msk);
  entry->deleteHeaderByte = (raw & SPW_ROUTER_TABLE_DELHEAD_Msk) != 0U;
}

void Spw_Router_setTableEntry(const uint8_t id, const Spw_Router_TableEntry* const entry)
{
  SPW_ROUTER_RoutingTableEntrySet(
    id, entry->deleteHeaderByte, (SPW_ROUTER_PHYS_ADDR)entry->address);
}
