// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// @file rx.h
/// @brief Header for the n7s Spacewire (Spw) RX driver wrapper.

#ifndef N7S_SPW_RX_H
#define N7S_SPW_RX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "microchip_spw/plib_spw_pktrx.h"

/// @brief Structure representing a Spacewire rx instance.
typedef struct {
  uint32_t dummy; // Placeholder member, currently unused
} Spw_Rx;

/// @brief Enumeration listing possible interrupts.
typedef enum {
  /// @brief Receive buffer activated.
  Spw_Rx_Irq_ReceivedBufferActivated = SPW_PKTRX_INT_MASK_ACT,
  /// @brief An incoming packet was discarded.
  Spw_Rx_Irq_IncomingPacketDiscarded = SPW_PKTRX_INT_MASK_DISCARD,
  /// @brief An incoming EEP detected.
  Spw_Rx_Irq_IncomingEepDetected = SPW_PKTRX_INT_MASK_EEP,
  /// @brief An incoming EOP detected.
  Spw_Rx_Irq_IncomingEopDetected = SPW_PKTRX_INT_MASK_EOP,
  /// @brief Receive buffer deactivated.
  Spw_Rx_Irq_ReceivedBufferDeactivated = SPW_PKTRX_INT_MASK_DEACT
} Spw_Rx_Irq;

/// @brief Structure listing triggered interrupts.
typedef struct {
  /// @brief Whether a receive buffer activated interrupt is pending.
  bool receivedBufferActivatedIrqOccurred;
  /// @brief Whether an incoming packet discarded interrupt is pending.
  bool incomingPacketDiscardedIrqOccurred;
  /// @brief Whether an incoming Eep detected interrupt is pending.
  bool incomingEepDetectedIrqOccurred;
  /// @brief Whether an incoming Eop detected interrupt is pending.
  bool incomingEopDetectedIrqOccurred;
  /// @brief Whether a receive buffer deactivated interrupt is pending.
  bool receivedBufferDeactivatedIrqOccurred;
} Spw_Rx_IrqStatus;

/// @brief Structure describing PKTRX status.
typedef struct {
  /// @brief Is receive buffer ready to be deactivated.
  bool isReceiveBufferReadyToDeactivate;
  /// @brief Is next Receive Buffer pending.
  bool isNextReceiveBufferPending;
  /// @brief Is current Receive Buffer active.
  bool isCurrentReceiveBufferActive;
  /// @brief Is next Receive Buffer armed.
  bool isNextReceiveBufferArmed;
  /// @brief Is previous receive buffer locked.
  bool isPreviousReceiveBufferLocked;
  /// @brief Is incoming Packet ongoing.
  bool isIncomingPacketOngoing;
  /// @brief Number of packets in current Receive Buffer.
  uint16_t numOfPacketsInActiveBuffer;
} Spw_Rx_Status;

/// @brief Structure describing previous Receive Buffer.
typedef struct {
  /// @brief Previous buffer was locked.
  bool isBufferLocked;
  /// @brief DMA Error during packet write.
  bool hasDmaErrorOccurred;
  /// @brief Receive Buffer data has become full.
  bool isBufferDataFull;
  /// @brief Receive Buffer information has become full.
  bool isBufferInfoFull;
  /// @brief An incoming Eep detected in this buffer.
  bool wasEepReceived;
  /// @brief Number of packets received in previous buffer
  uint16_t receivedPackets;
} Spw_Rx_PreviousRxBufferStatus;

/// @brief Enumeration listing possible overrunning packet policy.
typedef enum {
  /// @brief Stalls overrunning packet.
  Spw_Rx_OverrunningPacketPolicy_Stall = 0,
  /// @brief Discards overrunning packet.
  Spw_Rx_OverrunningPacketPolicy_Discard = 1
} Spw_Rx_OverrunningPacketPolicy;

/// @brief Enumeration listing possible Receive Buffer start conditions.
typedef enum {
  /// @brief Start if any bit in Start Value matches an incoming event.
  Spw_Rx_StartCondition_StartValue = 0,
  /// @brief Start immediately, if possible.
  Spw_Rx_StartCondition_StartNow = 1,
  /// @brief Start if Start Value matches an incoming Time Code from Time Code Handler 1
  Spw_Rx_StartCondition_StartRx1 = 2,
  /// @brief Start when current Receive Buffer is deactivated
  Spw_Rx_StartCondition_CurrentBufferDeactivated = 4,
  /// @brief Force the compiler to reserve 32-bit memory for enum
  Spw_Rx_StartCondition_Invalid = 0xFFFFFFFFU
} Spw_Rx_StartCondition;

/// @brief PktRx configuration structure.
typedef struct {
  /// @brief Overrunning packet policy config.
  Spw_Rx_OverrunningPacketPolicy overrunningPacketPolicy;
  /// @brief Interrupt mask for enable.
  uint32_t irqMaskEnable;
  /// @brief Interrupt mask for disable.
  uint32_t irqMaskDisable;
} Spw_Rx_Config;

/// @brief Receive Buffer entry decoded structure.
typedef struct {
  /// @brief Crc value.
  uint8_t crc;
  /// @brief Is packet split and continues in next entry/buffer.
  bool isPacketSplit;
  /// @brief Is packet continued from previous.
  bool isPacketContinued;
  /// @brief Was Eep received.
  bool wasEepReceived;
  /// @brief Was Eop received.
  bool wasEopReceived;
  /// @brief Start Address of received packet. Will always be word aligned.
  uint8_t* dataAddress;
  /// @brief Length of received packet, in bytes.
  uint32_t dataLength;
  /// @brief Time at end of packet, from receive buffer start. Time Unit is environment dependent.
  uint32_t packetTime;
} Spw_Rx_RxBufferEntryStruct;

/// @brief Receive Buffer entry buffer.
typedef SPW_PKTRX_INFO Spw_Rx_RxBufferEntry;

/// @brief Receive Buffer configuration structure.
typedef struct {
  /// @brief If a start condition is met, split any ongoing packet and deactivate.
  bool isPacketSplitAndDeactivationEnabled;
  /// @brief Receive Buffer start condition.
  Spw_Rx_StartCondition startCondition;
  /// @brief Receive Buffer start value if Spw_Rx_StartCondition_StartValue is used.
  uint8_t startValue;
  /// @brief Receive Buffer entry buffers address.
  Spw_Rx_RxBufferEntry* rxBufferAddress;
  /// @brief Number of Receive Buffer entry buffers.
  uint16_t rxBufferLength;
  /// @brief Received packet data buffer address.
  uint8_t* rxDataAddress;
  /// @brief Length of received data in bytes.
  uint32_t rxDataLength;
} Spw_Rx_RxBufferConfig;

/// @brief Initializes a device descriptor for Spacewire rx.
/// @param[in] rx   Rx descriptor.
void Spw_Rx_init(Spw_Rx* const rx);

/// @brief Sets Spacewire PktRx config.
/// @param[in] rx     Rx descriptor.
/// @param[in] config Rx configuration.
void Spw_Rx_setConfig(Spw_Rx* const rx, const Spw_Rx_Config* const config);

/// @brief Resets Spacewire PktRx to default state.
/// @param[out] rx     Rx descriptor.
void Spw_Rx_reset(Spw_Rx* const rx);

/// @brief Gets PktRx interrupts.
/// @param[in] rx     Rx descriptor.
uint32_t Spw_Rx_getIrq(const Spw_Rx* const rx);

/// @brief Gets masked PktRx interrupts.
/// @param[in] rx     Rx descriptor.
uint32_t Spw_Rx_getIrqMasked(const Spw_Rx* const rx);

/// @brief Gets and clears PktRx interrupts.
/// @param[out] rx     Rx descriptor.
uint32_t Spw_Rx_getAndClearIrq(Spw_Rx* const rx);

/// @brief Gets and clears masked PktRx interrupts.
/// @param[out] rx     Rx descriptor.
uint32_t Spw_Rx_getAndClearIrqMasked(Spw_Rx* const rx);

/// @brief Clears PktRx interrupts.
/// @param[out] rx     Rx descriptor.
void Spw_Rx_clearIrq(Spw_Rx* const rx);

/// @brief Sets next Receive Buffer.
/// @param[out] rx     Rx descriptor.
/// @param[in] config  Receive Buffer configuration.
void Spw_Rx_setNextRxBuffer(Spw_Rx* const rx, const Spw_Rx_RxBufferConfig* const config);

/// @brief Aborts current Receive Buffer.
/// @param[out] rx     Rx descriptor.
void Spw_Rx_abortOngoingPacketRx(Spw_Rx* const rx);

/// @brief Gets next Receive Buffer config.
/// @param[in] rx     Rx descriptor.
/// @param[out] config  Receive Buffer configuration.
void Spw_Rx_getNextRxBufferConfig(const Spw_Rx* const rx, Spw_Rx_RxBufferConfig* const config);

/// @brief Gets PktRx status.
/// @param[in] rx     Rx descriptor.
/// @param[out] status PktRx status.
void Spw_Rx_getStatus(const Spw_Rx* const rx, Spw_Rx_Status* const status);

/// @brief Gets previous Receive Buffer status.
/// @param[in] rx     Rx descriptor.
/// @param[out] status Previous Receive Buffer status.
void Spw_Rx_getPreviousRxBufferStatus(const Spw_Rx* const rx,
  Spw_Rx_PreviousRxBufferStatus* const status);

/// @brief Decodes Receive Buffer entry.
/// @param[in] entry  Receive Buffer entry.
/// @param[out] entryStruct Decoded Receive Buffer entry structure.
void Spw_Rx_getRxBufferEntry(const Spw_Rx_RxBufferEntry* const entry,
  Spw_Rx_RxBufferEntryStruct* const entryStruct);

/// @brief Decodes PktRx interrupt register.
/// @param[in] rxIrqStatusValue PktRx interrupt status value.
/// @param[out] status Decoded PktRx interrupt status.
void Spw_Rx_getStructuredIrq(const uint32_t rxIrqStatusValue, Spw_Rx_IrqStatus* const status);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // N7S_SPW_RX_H
