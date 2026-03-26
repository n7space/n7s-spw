// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

/// \file link.h
/// \brief Header for the n7s Spacewire (Spw) link driver wrapper.

#ifndef N7S_SPW_LINK_H
#define N7S_SPW_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "microchip_spw/plib_spw_link.h"

/// \brief Spacewire Link descriptor.
typedef struct {
	/// @brief Underlying Microchip link identifier (SPW_LINK_1 or
	/// SPW_LINK_2).
	SPW_LINK id;
} Spw_Link;

/// \brief Spacewire Link escape character event configuration.
typedef struct {
	/// @brief Whether the event is active or not
	bool active;
	/// @brief Bit mask applied to the incoming escape character.
	uint8_t mask;
	/// @brief Value to compare with the masked incoming escape character.
	uint8_t value;
} Spw_Link_EscCharEvent;

/// \brief Spacewire Link configuration.
typedef struct {
	/// @brief TX clock divisor for the initial sequence (CLKDIV.TXINITDIV).
	uint8_t txInitDiv;
	/// @brief TX clock divisor for normal operation (CLKDIV.TXOPERDIV).
	uint8_t txOperDiv;
	/// @brief Link start command (CFG.COMMAND): 0=disable, 1=start,
	/// 2=listen, 3=start+listen.
	uint8_t command;
	/// @brief Escape character event 1 configuration.
	Spw_Link_EscCharEvent escEvent1;
	/// @brief Escape character event 2 configuration.
	Spw_Link_EscCharEvent escEvent2;
	/// @brief Escape character to transmit
	uint8_t escChar;
	/// @brief Distributed interrupt bits to enable
	/// (SPW_LINK_DistInterruptEnable()).
	uint32_t distributedIrqToEnable;
	/// @brief Distributed interrupt bits to disable
	/// (SPW_LINK_DistInterruptDisable()).
	uint32_t distributedIrqToDisable;
	/// @brief Distributed interrupt ACK bits to enable
	/// (SPW_LINK_DistAckInterruptEnable()).
	uint32_t distributedAckIrqToEnable;
	/// @brief Distributed interrupt ACK bits to disable
	/// (SPW_LINK_DistAckInterruptDisable()).
	uint32_t distributedAckIrqToDisable;
} Spw_Link_Config;

/// \brief Decoded Spacewire Link interrupt status.
typedef struct {
	/// @brief Whether a disconnect error interrupt is pending.
	bool disErr;
	/// @brief Whether a parity error interrupt is pending.
	bool parErr;
	/// @brief Whether an escape error interrupt is pending.
	bool escErr;
	/// @brief Whether a credit error interrupt is pending.
	bool crErr;
	/// @brief Whether a link abort interrupt is pending.
	bool linkAbort;
	/// @brief Whether an EEP transmitted interrupt is pending.
	bool eepTrans;
	/// @brief Whether an EEP received interrupt is pending.
	bool eepRec;
	/// @brief Whether a packet discarded interrupt is pending.
	bool discard;
	/// @brief Whether an escape character match event 2 interrupt is
	/// pending.
	bool escEvent2;
	/// @brief Whether an escape character match event 1 interrupt is
	/// pending.
	bool escEvent1;
} Spw_Link_IrqStatus;

/// \brief Enumeration listing possible Spw Link states.
typedef enum {
	/// \brief Link in ErrorReset state.
	Spw_Link_State_ErrorReset = 0,
	/// \brief Link in ErrorWait state.
	Spw_Link_State_ErrorWait = 1,
	/// \brief Link in Ready state.
	Spw_Link_State_Ready = 2,
	/// \brief Link in Started state.
	Spw_Link_State_Started = 3,
	/// \brief Link in Connecting state.
	Spw_Link_State_Connecting = 4,
	/// \brief Link in Run state.
	Spw_Link_State_Run = 5,
} Spw_Link_State;

/// \brief Structure describing Spw Link status.
typedef struct {
	/// \brief Link was/is in Run state.
	bool wasRunStateSeen;
	/// \brief Link was/is in Connecting state.
	bool wasConnectingStateSeen;
	/// \brief Link was/is in Started state.
	bool wasStartedStateSeen;
	/// \brief Link was/is in Ready state.
	bool wasReadyStateSeen;
	/// \brief Link was/is in ErrorWait state.
	bool wasErrorWaitStateSeen;
	/// \brief Link was/is in ErrorReset state.
	bool wasErrorResetStateSeen;
	/// \brief Receiver got N-chars.
	bool hasReceiverGotNchars;
	/// \brief Receiver got FCT.
	bool hasReceiverGotFct;
	/// \brief Receiver got Null character.
	bool hasReceiverGotNull;
	/// \brief The transmit buffer is empty.
	bool isTxBufferEmpty;
	/// \brief The strapped reset value for transmitter bitrates.
	uint8_t strappedResetValueForTxBitrates;
	/// \brief Current Link state
	Spw_Link_State linkState;
	/// \brief Last matched Escape Character Event 1
	uint8_t lastMatchingEscCharEvent1;
	/// \brief Last matched Escape Character Event 2
	uint8_t lastMatchingEscCharEvent2;
} Spw_Link_Status;

/// \brief Initializes a Spacewire Link descriptor and configures the hardware.
/// \param[out] link   Descriptor to initialize.
/// \param[in]  id     Link to use (SPW_LINK_1 or SPW_LINK_2).
/// setup).
void Spw_Link_init(Spw_Link *const link, const SPW_LINK id);

/// \brief Applies a new configuration to the Spacewire Link hardware.
/// \param[in] link   Link descriptor.
/// \param[in] config Configuration to apply.
void Spw_Link_setConfig(const Spw_Link *const link,
			const Spw_Link_Config *const config);

/// \brief Reads the current Spacewire Link hardware configuration.
/// \param[in]  link   Link descriptor.
/// \param[out] config Populated with the current register values.
void Spw_Link_getConfig(const Spw_Link *const link,
			Spw_Link_Config *const config);

/// \brief Issues a software reset to the Spacewire Link.
/// \param[in] link Link descriptor.
void Spw_Link_reset(Spw_Link *const link);

/// \brief Reads the Spacewire Link status register.
/// \param[in] link Link descriptor.
/// \param[out] status Decoded link status.
void Spw_Link_getStatus(const Spw_Link *const link,
			Spw_Link_Status *const status);

/// \brief Enables and/or disables Spacewire Link interrupts.
/// \param[in] link    Link descriptor.
/// \param[in] enable  Interrupt bits to enable.
/// \param[in] disable Interrupt bits to disable.
void Spw_Link_setIrqMask(Spw_Link *const link, uint32_t enable, uint32_t disable);

/// \brief Reads the raw (unmasked) pending Spacewire Link interrupts.
/// \param[in] link Link descriptor.
/// \returns Raw pending interrupt status (PI_R register).
uint32_t Spw_Link_getIrq(const Spw_Link *const link);

/// \brief Reads the masked pending Spacewire Link interrupts.
/// \param[in] link Link descriptor.
/// \returns Masked pending interrupt status (PI_RM register).
uint32_t Spw_Link_getIrqMasked(const Spw_Link *const link);

/// \brief Reads the raw (unmasked) pending interrupts and clears them.
/// \param[in] link Link descriptor.
/// \returns Raw pending interrupt status at the time of the read.
uint32_t Spw_Link_getAndClearIrq(const Spw_Link *const link);

/// \brief Reads the masked pending interrupts and clears them.
/// \param[in] link Link descriptor.
/// \returns Masked pending interrupt status.
uint32_t Spw_Link_getAndClearIrqMasked(const Spw_Link *const link);

/// \brief Clears selected Spacewire Link pending interrupts.
/// \param[in] link Link descriptor.
/// \param[in] mask Bits to clear.
void Spw_Link_clearIrq(Spw_Link *const link, uint32_t mask);

/// \brief Decodes a raw interrupt status into a structured representation.
/// \param[in]  irq    Raw interrupt status (e.g. from Spw_Link_getIrq()).
/// \param[out] status Decoded interrupt fields.
void Spw_Link_getStructuredIrq(uint32_t irq, Spw_Link_IrqStatus *const status);

/// \brief Enables and/or disables Spacewire Link distributed interrupts.
/// \param[in] link    Link descriptor.
/// \param[in] enable  Distributed interrupt bits to enable.
/// \param[in] disable Distributed interrupt bits to disable.
void Spw_Link_setDistributedInterruptIrqMask(Spw_Link *const link,
					     const uint32_t enable,
					     const uint32_t disable);

/// \brief Reads raw (unmasked) Distributed Interrupt pending interrupts.
/// \param[in] link Link descriptor.
/// \returns Raw distributed interrupt pending status.
uint32_t Spw_Link_getDistributedInterruptIrq(const Spw_Link *const link);

/// \brief Reads masked Distributed Interrupt pending interrupts.
/// \param[in] link Link descriptor.
/// \returns Masked distributed interrupt pending status.
uint32_t Spw_Link_getDistributedInterruptIrqMasked(const Spw_Link *const link);

/// \brief Reads raw Distributed Interrupt pending interrupts and clears them.
/// \param[in] link Link descriptor.
/// \returns Raw distributed interrupt pending status.
uint32_t Spw_Link_getAndClearDistributedInterruptIrq(const Spw_Link *const link);

/// \brief Reads masked Distributed Interrupt pending interrupts and clears
/// them.
/// \param[in] link Link descriptor.
/// \returns Masked distributed interrupt pending status.
uint32_t
Spw_Link_getAndClearDistributedInterruptIrqMasked(const Spw_Link *const link);

/// \brief Clears selected Distributed Interrupt pending interrupts.
/// \param[in] link Link descriptor.
/// \param[in] mask Bits to clear.
void Spw_Link_clearDistributedInterruptIrq(Spw_Link *const link,
					   const uint32_t mask);

/// \brief Enables and/or disables Spacewire Link Distributed Interrupt
/// Acknowledge interrupts.
/// \param[in] link    Link descriptor.
/// \param[in] enable  Bits to enable.
/// \param[in] disable Bits to disable.
void Spw_Link_setDistributedInterruptAckIrqMask(Spw_Link *const link,
						const uint32_t enable,
						const uint32_t disable);

/// \brief Reads raw (unmasked) Distributed Interrupt Acknowledge pending
/// interrupts.
/// \param[in] link Link descriptor.
/// \returns Raw distributed acknowledge interrupt pending status.
uint32_t Spw_Link_getDistributedInterruptAckIrq(const Spw_Link *const link);

/// \brief Reads masked Distributed Interrupt Acknowledge pending interrupts.
/// \param[in] link Link descriptor.
/// \returns Masked distributed acknowledge interrupt pending status.
uint32_t Spw_Link_getDistributedInterruptAckIrqMasked(const Spw_Link *const link);

/// \brief Reads raw Distributed Interrupt Acknowledge pending interrupts and
/// clears them.
/// \param[in] link Link descriptor.
/// \returns Raw distributed acknowledge pending status.
uint32_t Spw_Link_getAndClearDistributedInterruptAckIrq(const Spw_Link *const link);

/// \brief Reads masked Distributed Interrupt Acknowledge pending interrupts and
/// clears them.
/// \param[in] link Link descriptor.
/// \returns Masked distributed acknowledge pending status.
uint32_t
Spw_Link_getAndClearDistributedInterruptAckIrqMasked(const Spw_Link *const link);

/// \brief Clears selected Distributed Interrupt Acknowledge pending interrupts.
/// \param[in] link Link descriptor.
/// \param[in] mask Bits to clear.
void Spw_Link_clearDistributedInterruptAckIrq(Spw_Link *const link,
					      const uint32_t mask);

/// \brief Configures Escape Character Match Event 1.
/// \param[in] link  Link descriptor.
/// \param[in] event Event configuration (set active=false to disable).
void Spw_Link_setEscapeCharEvent1(Spw_Link *const link,
				  const Spw_Link_EscCharEvent *const event);

/// \brief Configures Escape Character Match Event 2.
/// \param[in] link  Link descriptor.
/// \param[in] event Event configuration (set active=false to disable).
void Spw_Link_setEscapeCharEvent2(Spw_Link *const link,
				  const Spw_Link_EscCharEvent *const event);

/// \brief Returns the last received escape character that matched Event 1.
/// \param[in] link Link descriptor.
/// \returns Last matched escape character byte.
uint8_t Spw_Link_getLastEscapeCharEvent1(const Spw_Link *const link);

/// \brief Returns the last received escape character that matched Event 2.
/// \param[in] link Link descriptor.
/// \returns Last matched escape character byte.
uint8_t Spw_Link_getLastEscapeCharEvent2(const Spw_Link *const link);

/// \brief Sets the escape character to be transmitted on the Spacewire Link.
/// \param[in] link    Link descriptor.
/// \param[in] escChar New escape character to transmit.
void Spw_Link_setTransmitEscapeChar(Spw_Link *const link, const uint8_t escChar);

#ifdef __cplusplus
} // extern "C"
#endif

/// @}

#endif // N7S_SPW_LINK_H