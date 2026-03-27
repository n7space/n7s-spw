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
#include "stdbool.h"

#include "microchip_spw/plib_spw_pkttx.h"

/// \brief Structure representing a Spacewire tx instance.
typedef struct {
  uint32_t dummy;
} Spw_Tx;

/// \brief Spw Send List tx condition.
typedef enum {
  /// \brief Start if any bit in Start Value matches an incoming event.
  Spw_Tx_SendCondition_StartValueMatch = 0U,
  /// \brief Start immediately, if possible.
  Spw_Tx_SendCondition_StartNow = 1U,
  /// \brief Start if Start Value matches an incoming Time Code from Time Code Handler 1
  Spw_Tx_SendCondition_StartValueMatchTch1 = 2U,
  /// \brief Force the compiler to reserve 32-bit memory for enum
  Spw_Tx_SendCondition_Invalid = 0xFFFFFFFFUL
} Spw_Tx_SendCondition;

/// \brief Packet entry type.
typedef enum {
  /// \brief Normal data packet.
  Spw_Tx_EntryType_PacketData = 0u,
  /// \brief Packet contains escape character.
  Spw_Tx_EntryType_EscapeCharacter = 1u,
} Spw_Tx_EntryType;

/// \brief Send List packet entry configuration structure.
typedef struct {
  /// \brief Spw skips this entry.
  bool isEntrySkipped;
  /// \brief Packet entry type.
  Spw_Tx_EntryType entryType;
  /// \brief Number of router bytes.
  uint8_t routerByteLength;
  /// \brief Delay from send list start. Time Unit is environment dependent.
  uint32_t startTime;
  /// \brief Router bytes to use. Number of used entries depends on routerByteLength field
  uint8_t routerByte[8];
  /// \brief Defines where to send Escape Character.
  uint8_t escapeCharMask;
  /// \brief Escape Character to send if Entry Type is Escape Char.
  uint8_t escapeChar;
  /// \brief Enable header CRC calculation.
  bool calculateHeaderCrc;
  /// \brief Header size in bytes.
  uint8_t headerSize;
  /// \brief Pointer to the header. Must be aligned to 4 bytes.
  uint8_t* headerAddress;
  /// \brief Enable data CRC calculation.
  bool calculateDataCrc;
  /// \brief Size of data field
  uint32_t dataSize;
  /// \brief Pointer to the data. Must be aligned to 4 bytes.
  uint8_t* dataAddress;
  /// \brief Abort send list if time from send list start exceeds this value. Write 0 to disable.
  uint32_t timeout;
} Spw_Tx_SendListEntryStruct;

/// \brief Send List entry buffer.
typedef SPW_PKTTX_SEND_LIST_ENTRY Spw_Tx_SendListEntry;

/// \brief Enumeration listing possible Deactivated Send List states.
typedef enum {
  /// \brief No information. Field not locked.
  Spw_Tx_DeactivatedSendListState_Noinfo = 0,
  /// \brief Last send list fully sent.
  Spw_Tx_DeactivatedSendListState_LastSendIsOk = 1,
  /// \brief Aborted due to memory access error.
  Spw_Tx_DeactivatedSendListState_AbortedMemoryAccessError = 2,
  /// \brief Aborted by new send list.
  Spw_Tx_DeactivatedSendListState_AbortedNewSendList = 3,
  /// \brief Aborted by direct user command.
  Spw_Tx_DeactivatedSendListState_AbortedByUser = 4,
  /// \brief Aborted by timeout.
  Spw_Tx_DeactivatedSendListState_Timeout = 5,
  /// \brief Number of possible states.
  Spw_Tx_DeactivatedSendListState_Count = 6
} Spw_Tx_DeactivatedSendListState;

/// \brief Structure describing PktTx status.
typedef struct {
  /// \brief Previous Send List state
  Spw_Tx_DeactivatedSendListState lastState;
  /// \brief Flag indicating whether current send list can be aborted.
  bool isReadyToDeactivateCurrentSendList;
  /// \brief Is next Send List pending.
  bool isNextSendListPending;
  /// \brief Is current Send List active.
  bool isCurrentSendListActive;
  /// \brief Is next Send List armed.
  bool isNextSendListArmed;
} Spw_Tx_Status;

/// \brief Send List configuration structure.
typedef struct {
  /// \brief Send List send condition.
  Spw_Tx_SendCondition sendCondition;
  /// \brief Number of entries to send in Send List.
  uint16_t sendListLength;
  /// \brief Pointer to the Send List entries.
  Spw_Tx_SendListEntry* sendListAddress;
  /// \brief Router bytes to add if non-zero.
  uint8_t routerByte[4];
  /// \brief Abort current Send List if new is started.
  bool abortOngoingSendListWhenStarted;
  /// \brief Activate on matching Start value. Used when with Spw_Tx_SendCondition_StartValueMatch.
  uint8_t startValue;
} Spw_Tx_SendListConfig;

/// \brief Enumeration listing possible PktTx interrupts to be enabled.
typedef enum {
  /// \brief Eep was sent.
  Spw_Tx_Irq_EepSent = SPW_PKTTX_INT_MASK_EEP,
  /// \brief Eop was sent.
  Spw_Tx_Irq_EopSent = SPW_PKTTX_INT_MASK_EOP,
  /// \brief Send List was activated.
  Spw_Tx_Irq_SendListActivated = SPW_PKTTX_INT_MASK_ACT,
  /// \brief Send List was deactivated.
  Spw_Tx_Irq_SendListDeactivated = SPW_PKTTX_INT_MASK_DEACT
} Spw_Tx_Irq;

/// \brief Structure listing triggered PktTx interrupts.
typedef struct {
  /// \brief Eep sent interrupt triggered.
  bool eepSentIrqOccurred;
  /// \brief Eop sent interrupt triggered.
  bool eopSentIrqOccurred;
  /// \brief Send List activated interrupt triggered.
  bool sendListActivatedIrqOccurred;
  /// \brief Send List deactivated interrupt triggered.
  bool sendListDeactivatedIrqOccurred;
} Spw_Tx_IrqStatus;

/// \brief PktTx configuration structure.
typedef struct {
  /// @brief Interrupt mask for enable.
	uint32_t irqMaskEnable;
	/// @brief Interrupt mask for disable.
	uint32_t irqMaskDisable;uint32_t irqMask;
} Spw_Tx_Config;

/// \brief Initializes a device descriptor for Spacewire tx.
void Spw_Tx_init(Spw_Tx* const tx);

/// \brief Sets Spacewire PktTx config.
void Spw_Tx_setConfig(Spw_Tx* const tx, const Spw_Tx_Config* const config);

/// \brief Resets Spacewire PktTx to default state.
void Spw_Tx_reset(const Spw_Tx* const tx);

/// \brief Sets next Send List.
void Spw_Tx_setNextSendList(Spw_Tx* const tx, const Spw_Tx_SendListConfig* const config);

/// \brief Sets next Send List entry.
void Spw_Tx_setNextSendListEntry(Spw_Tx_SendListEntry* const entry,
  const Spw_Tx_SendListEntryStruct* const entryStruct);

/// \brief Aborts transmission of current Send List.
void Spw_Tx_abortCurrentSendList(Spw_Tx* const tx);

/// \brief Gets current Send List Config.
void Spw_Tx_getCurrentSendListConfig(const Spw_Tx* const tx, Spw_Tx_SendListConfig* const config);

/// \brief Gets Send List entry structure.
void Spw_Tx_getSendListEntry(const Spw_Tx_SendListEntry* const entry,
  Spw_Tx_SendListEntryStruct* const entryStruct);

/// \brief Sets PktTx interrupt mask.
void Spw_Tx_setIrqMask(Spw_Tx* const tx, uint32_t mask);

/// \brief Gets PktTx interrupts.
uint32_t Spw_Tx_getIrq(const Spw_Tx* const tx);

/// \brief Gets masked PktTx interrupts.
uint32_t Spw_Tx_getIrqMasked(const Spw_Tx* const tx);

/// \brief Gets and clears PktTx interrupts.
uint32_t Spw_Tx_getAndClearIrq(const Spw_Tx* const tx);

/// \brief Gets and clears masked PktTx interrupts.
uint32_t Spw_Tx_getAndClearIrqMasked(const Spw_Tx* const tx);

/// \brief Clears PktTx interrupts.
void Spw_Tx_clearIrq(const Spw_Tx* const tx);

/// \brief Decodes PktTx interrupt status register to a structure.
void Spw_Tx_getStructuredIrq(const uint32_t linkIrqStatusValue, Spw_Tx_IrqStatus* const status);

/// \brief Gets PktTx status register.
void Spw_Tx_getStatus(Spw_Tx* const tx, Spw_Tx_Status* const status);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_TX_H