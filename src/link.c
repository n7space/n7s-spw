// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2026

#include "link.h"
#include "device.h"

#define SWRESET_ARM_PATTERN 0x4D616A6FU
#define SWRESET_TRIGGER_PATTERN 0x72546F6DU

static inline uint32_t readIrq(SPW_LINK id)
{
	if (id == SPW_LINK_1) {
		return SPW_REGS->SPW_LINK1_PI_R;
	} else if (id == SPW_LINK_2) {
		return SPW_REGS->SPW_LINK2_PI_R;
	}
}

static inline uint32_t readIrqMasked(SPW_LINK id)
{
	if (id == SPW_LINK_1) {
		return SPW_REGS->SPW_LINK1_PI_RM;
	} else if (id == SPW_LINK_2) {
		return SPW_REGS->SPW_LINK2_PI_RM;
	}
}

static inline void writeIrqClear(SPW_LINK id, uint32_t mask)
{
	if (id == SPW_LINK_1) {
		SPW_REGS->SPW_LINK1_PI_C = mask;
	} else {
		SPW_REGS->SPW_LINK2_PI_C = mask;
	}
}

static inline uint32_t readDistIrq(SPW_LINK id)
{
	if (id == SPW_LINK_1) {
		return SPW_REGS->SPW_LINK1_DISTINTPI_R;
	} else if (id == SPW_LINK_2) {
		return SPW_REGS->SPW_LINK2_DISTINTPI_R;
	}
}

static inline uint32_t readDistIrqMasked(SPW_LINK id)
{
	if (id == SPW_LINK_1) {
		return SPW_REGS->SPW_LINK1_DISTINTPI_RM;
	} else if (id == SPW_LINK_2) {
		return SPW_REGS->SPW_LINK2_DISTINTPI_RM;
	}
}

static inline void writeDistIrqClear(SPW_LINK id, uint32_t mask)
{
	if (id == SPW_LINK_1) {
		SPW_REGS->SPW_LINK1_DISTINTPI_C = mask;
	} else if (id == SPW_LINK_2) {
		SPW_REGS->SPW_LINK2_DISTINTPI_C = mask;
	}
}

static inline uint32_t readDistAckIrq(SPW_LINK id)
{
	if (id == SPW_LINK_1) {
		return SPW_REGS->SPW_LINK1_DISTACKPI_R;
	} else if (id == SPW_LINK_2) {
		return SPW_REGS->SPW_LINK2_DISTACKPI_R;
	}
}

static inline uint32_t readDistAckIrqMasked(SPW_LINK id)
{
	if (id == SPW_LINK_1) {
		return SPW_REGS->SPW_LINK1_DISTACKPI_RM;
	} else if (id == SPW_LINK_2) {
		return SPW_REGS->SPW_LINK2_DISTACKPI_RM;
	}
}

static inline void writeDistAckIrqClear(SPW_LINK id, uint32_t mask)
{
	if (id == SPW_LINK_1) {
		SPW_REGS->SPW_LINK1_DISTACKPI_C = mask;
	} else if (id == SPW_LINK_2) {
		SPW_REGS->SPW_LINK2_DISTACKPI_C = mask;
	}
}

void Spw_Link_init(Spw_Link *const link, SPW_LINK id,
		   const Spw_Link_Config *const config)
{
	Spw_Link_setIrqMask(link, 0, 0xFFFFFFFF);

	link->id = id;
	if (config != NULL) {
		Spw_Link_setConfig(link, config);
	}
}

void Spw_Link_setConfig(const Spw_Link *const link,
			const Spw_Link_Config *const config)
{
	if (link->id == SPW_LINK_1) {
		SPW_REGS->SPW_LINK1_CLKDIV =
			SPW_LINK1_CLKDIV_TXINITDIV(config->txInitDiv) |
			SPW_LINK1_CLKDIV_TXOPERDIV(config->txOperDiv);
		SPW_REGS->SPW_LINK1_CFG = SPW_LINK1_CFG_COMMAND(config->command);
	} else if (link->id == SPW_LINK_2) {
		SPW_REGS->SPW_LINK2_CLKDIV =
			SPW_LINK2_CLKDIV_TXINITDIV(config->txInitDiv) |
			SPW_LINK2_CLKDIV_TXOPERDIV(config->txOperDiv);
		SPW_REGS->SPW_LINK2_CFG = SPW_LINK2_CFG_COMMAND(config->command);
	}

	SPW_LINK_TransmitEscapeChar(link->id, config->escChar);

	SPW_LINK_EscapeCharEvent1Set(link->id, config->escEvent1.active,
				     config->escEvent1.mask,
				     config->escEvent1.value);
	SPW_LINK_EscapeCharEvent2Set(link->id, config->escEvent2.active,
				     config->escEvent2.mask,
				     config->escEvent2.value);

	SPW_LINK_DistInterruptEnable(link->id, config->distributedIrqToEnable);
	SPW_LINK_DistInterruptDisable(link->id, config->distributedIrqToDisable);

	SPW_LINK_DistAckInterruptEnable(link->id,
					config->distributedAckIrqToEnable);
	SPW_LINK_DistAckInterruptDisable(link->id,
					 config->distributedAckIrqToDisable);
}

void Spw_Link_getConfig(const Spw_Link *const link, Spw_Link_Config *const config)
{
	uint32_t clkdiv;
	uint32_t cfg;

	if (link->id == SPW_LINK_1) {
		clkdiv = SPW_REGS->SPW_LINK1_CLKDIV;
		cfg    = SPW_REGS->SPW_LINK1_CFG;
	} else if (link->id == SPW_LINK_2) {
		clkdiv = SPW_REGS->SPW_LINK2_CLKDIV;
		cfg    = SPW_REGS->SPW_LINK2_CFG;
	}

	config->txInitDiv = (uint8_t)((clkdiv & SPW_LINK1_CLKDIV_TXINITDIV_Msk) >>
				      SPW_LINK1_CLKDIV_TXINITDIV_Pos);
	config->txOperDiv = (uint8_t)((clkdiv & SPW_LINK1_CLKDIV_TXOPERDIV_Msk) >>
				      SPW_LINK1_CLKDIV_TXOPERDIV_Pos);
	config->command = (uint8_t)((cfg & SPW_LINK1_CFG_COMMAND_Msk) >>
				    SPW_LINK1_CFG_COMMAND_Pos);

	config->escEvent1.active = (SPW_REGS->SPW_LINK1_ESCCHAREVENT0 &
				    SPW_LINK1_ESCCHAREVENT0_ACTIVE_Msk) != 0U;
	config->escEvent1.mask	= (uint8_t)((SPW_REGS->SPW_LINK1_ESCCHAREVENT0 &
					     SPW_LINK1_ESCCHAREVENT0_MASK_Msk) >>
					    SPW_LINK1_ESCCHAREVENT0_MASK_Pos);
	config->escEvent1.value = (uint8_t)((SPW_REGS->SPW_LINK1_ESCCHAREVENT0 &
					     SPW_LINK1_ESCCHAREVENT0_VALUE_Msk) >>
					    SPW_LINK1_ESCCHAREVENT0_VALUE_Pos);
	config->escEvent2.active = (SPW_REGS->SPW_LINK1_ESCCHAREVENT1 &
				    SPW_LINK1_ESCCHAREVENT1_ACTIVE_Msk) != 0U;
	config->escEvent2.mask	= (uint8_t)((SPW_REGS->SPW_LINK1_ESCCHAREVENT1 &
					     SPW_LINK1_ESCCHAREVENT1_MASK_Msk) >>
					    SPW_LINK1_ESCCHAREVENT1_MASK_Pos);
	config->escEvent2.value = (uint8_t)((SPW_REGS->SPW_LINK1_ESCCHAREVENT1 &
					     SPW_LINK1_ESCCHAREVENT1_VALUE_Msk) >>
					    SPW_LINK1_ESCCHAREVENT1_VALUE_Pos);

	config->escChar = (uint8_t)((SPW_REGS->SPW_LINK1_TRANSESC &
				     SPW_LINK1_TRANSESC_CHAR_Msk) >>
				    SPW_LINK1_TRANSESC_CHAR_Pos);

	config->distributedIrqToEnable =
		0; // No way to read currently enabled interrupts
	config->distributedIrqToDisable =
		0; // No way to read currently enabled interrupts
	config->distributedAckIrqToEnable =
		0; // No way to read currently enabled interrupts
	config->distributedAckIrqToDisable =
		0; // No way to read currently enabled interrupts
}

void Spw_Link_reset(Spw_Link *const link)
{
	if (link->id == SPW_LINK_1) {
		SPW_REGS->SPW_LINK1_SWRESET = SWRESET_ARM_PATTERN;
		// Generate read from SWRESET register (without it resets are
		// unreliable)
		(void)SPW_REGS->SPW_LINK1_SWRESET;
		SPW_REGS->SPW_LINK1_SWRESET = SWRESET_TRIGGER_PATTERN;
	} else if (link->id == SPW_LINK_2) {
		SPW_REGS->SPW_LINK2_SWRESET = SWRESET_ARM_PATTERN;
		// Generate read from SWRESET register (without it resets are
		// unreliable)
		(void)SPW_REGS->SPW_LINK2_SWRESET;
		SPW_REGS->SPW_LINK2_SWRESET = SWRESET_TRIGGER_PATTERN;
	}
}

void Spw_Link_getStatus(const Spw_Link *const link, Spw_Link_Status *const status)
{
	SPW_LINK_STATUS rawStatus = SPW_LINK_StatusGet(link->id);

	status->wasRunStateSeen = (rawStatus & SPW_LINK_STATUS_SEEN5) != 0U;
	status->wasConnectingStateSeen = (rawStatus & SPW_LINK_STATUS_SEEN4) !=
					 0U;
	status->wasStartedStateSeen = (rawStatus & SPW_LINK_STATUS_SEEN3) != 0U;
	status->wasReadyStateSeen   = (rawStatus & SPW_LINK_STATUS_SEEN2) != 0U;
	status->wasErrorWaitStateSeen = (rawStatus & SPW_LINK_STATUS_SEEN1) != 0U;
	status->wasErrorResetStateSeen = (rawStatus & SPW_LINK_STATUS_SEEN0) !=
					 0U;
	status->hasReceiverGotNchars = (rawStatus & SPW_LINK_STATUS_GOTNCHAR) !=
				       0U;
	status->hasReceiverGotFct = (rawStatus & SPW_LINK_STATUS_GOTFCT) != 0U;
	status->hasReceiverGotNull = (rawStatus & SPW_LINK_STATUS_GOTNULL) != 0U;
	status->isTxBufferEmpty = (rawStatus & SPW_LINK_STATUS_TXEMPTY) != 0U;
	status->strappedResetValueForTxBitrates =
		(uint8_t)((rawStatus & SPW_LINK_STATUS_TXDEFDIV_MASK) >>
			  SPW_LINK1_STATUS_TXDEFDIV_Pos);
	status->linkState = (Spw_Link_State)((rawStatus &
					      SPW_LINK_STATUS_LINKSTATE_MASK) >>
					     SPW_LINK1_STATUS_LINKSTATE_Pos);
	status->lastMatchingEscCharEvent1 = SPW_LINK_LastRecvEscapeCharEvent1Get(
		link->id);
	status->lastMatchingEscCharEvent2 = SPW_LINK_LastRecvEscapeCharEvent2Get(
		link->id);
}

void Spw_Link_setIrqMask(Spw_Link *const link, const uint32_t enable,
			 const uint32_t disable)
{
	if (enable != 0U) {
		SPW_LINK_InterruptEnable(link->id, enable);
	}
	if (disable != 0U) {
		SPW_LINK_InterruptDisable(link->id, disable);
	}
}

uint32_t Spw_Link_getIrq(const Spw_Link *const link)
{
	return (uint32_t)readIrq(link->id);
}

uint32_t Spw_Link_getIrqMasked(const Spw_Link *const link)
{
	return (uint32_t)readIrqMasked(link->id);
}

uint32_t Spw_Link_getAndClearIrq(const Spw_Link *const link)
{
	uint32_t pending = readIrq(link->id);
	writeIrqClear(link->id, pending);
	return (uint32_t)pending;
}

uint32_t Spw_Link_getAndClearIrqMasked(const Spw_Link *const link)
{
	return SPW_LINK_IrqStatusGetMaskedAndClear(link->id);
}

void Spw_Link_clearIrq(Spw_Link *const link, const uint32_t mask)
{
	writeIrqClear(link->id, (uint32_t)mask);
}

void Spw_Link_getStructuredIrq(const uint32_t irq,
			       Spw_Link_IrqStatus *const status)
{
	status->disErr	  = ((irq & SPW_LINK_INT_MASK_DISERR) != 0U);
	status->parErr	  = ((irq & SPW_LINK_INT_MASK_PARERR) != 0U);
	status->escErr	  = ((irq & SPW_LINK_INT_MASK_ESCERR) != 0U);
	status->crErr	  = ((irq & SPW_LINK_INT_MASK_CRERR) != 0U);
	status->linkAbort = ((irq & SPW_LINK_INT_MASK_LINKABORT) != 0U);
	status->eepTrans  = ((irq & SPW_LINK_INT_MASK_EEPTRANS) != 0U);
	status->eepRec	  = ((irq & SPW_LINK_INT_MASK_EEPREC) != 0U);
	status->discard	  = ((irq & SPW_LINK_INT_MASK_DISCARD) != 0U);
	status->escEvent2 = ((irq & SPW_LINK_INT_MASK_ESCEVENT2) != 0U);
	status->escEvent1 = ((irq & SPW_LINK_INT_MASK_ESCEVENT1) != 0U);
}

void Spw_Link_setDistributedInterruptIrqMask(Spw_Link *const link,
					     const uint32_t enable,
					     const uint32_t disable)
{
	if (enable != 0U) {
		SPW_LINK_DistInterruptEnable(link->id, enable);
	}
	if (disable != 0U) {
		SPW_LINK_DistInterruptDisable(link->id, disable);
	}
}

uint32_t Spw_Link_getDistributedInterruptIrq(const Spw_Link *const link)
{
	return (uint32_t)readDistIrq(link->id);
}

uint32_t Spw_Link_getDistributedInterruptIrqMasked(const Spw_Link *const link)
{
	return (uint32_t)readDistIrqMasked(link->id);
}

uint32_t Spw_Link_getAndClearDistributedInterruptIrq(const Spw_Link *const link)
{
	uint32_t pending = readDistIrq(link->id);
	writeDistIrqClear(link->id, pending);
	return (uint32_t)pending;
}

uint32_t
Spw_Link_getAndClearDistributedInterruptIrqMasked(const Spw_Link *const link)
{
	return SPW_LINK_DistIrqStatusGetMaskedAndClear(link->id);
}

void Spw_Link_clearDistributedInterruptIrq(Spw_Link *const link,
					   const uint32_t mask)
{
	writeDistIrqClear(link->id, (uint32_t)mask);
}

void Spw_Link_setDistributedInterruptAckIrqMask(Spw_Link *const link,
						const uint32_t enable,
						const uint32_t disable)
{
	if (enable != 0U) {
		SPW_LINK_DistAckInterruptEnable(link->id, enable);
	}
	if (disable != 0U) {
		SPW_LINK_DistAckInterruptDisable(link->id, disable);
	}
}

uint32_t Spw_Link_getDistributedInterruptAckIrq(const Spw_Link *const link)
{
	return (uint32_t)readDistAckIrq(link->id);
}

uint32_t Spw_Link_getDistributedInterruptAckIrqMasked(const Spw_Link *const link)
{
	return (uint32_t)readDistAckIrqMasked(link->id);
}

uint32_t Spw_Link_getAndClearDistributedInterruptAckIrq(const Spw_Link *const link)
{
	uint32_t pending = readDistAckIrq(link->id);
	writeDistAckIrqClear(link->id, pending);
	return (uint32_t)pending;
}

uint32_t
Spw_Link_getAndClearDistributedInterruptAckIrqMasked(const Spw_Link *const link)
{
	return SPW_LINK_DistAckIrqStatusGetMaskedAndClear(link->id);
}

void Spw_Link_clearDistributedInterruptAckIrq(Spw_Link *const link,
					      const uint32_t mask)
{
	writeDistAckIrqClear(link->id, (uint32_t)mask);
}

void Spw_Link_setEscapeCharEvent1(Spw_Link *const link,
				  const Spw_Link_EscCharEvent *const event)
{
	SPW_LINK_EscapeCharEvent1Set(link->id, event->active, event->mask,
				     event->value);
}

void Spw_Link_setEscapeCharEvent2(Spw_Link *const link,
				  const Spw_Link_EscCharEvent *const event)
{
	SPW_LINK_EscapeCharEvent2Set(link->id, event->active, event->mask,
				     event->value);
}

uint8_t Spw_Link_getLastEscapeCharEvent1(const Spw_Link *const link)
{
	return SPW_LINK_LastRecvEscapeCharEvent1Get(link->id);
}

uint8_t Spw_Link_getLastEscapeCharEvent2(const Spw_Link *const link)
{
	return SPW_LINK_LastRecvEscapeCharEvent2Get(link->id);
}

void Spw_Link_setTransmitEscapeChar(Spw_Link *const link, const uint8_t escChar)
{
	SPW_LINK_TransmitEscapeChar(link->id, escChar);
}