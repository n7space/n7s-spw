// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2025

#include "rx.h"

#define SWRESET_ARM_PATTERN 0x4D616A6FU
#define SWRESET_TRIGGER_PATTERN 0x72546F6DU

#define SPW_PKTRX1_IRQREGS_ALLIRQ_MASK 0x0000001FU

void Spw_Rx_init(Spw_Rx *const rx) { (void)rx; }

void Spw_Rx_setConfig(Spw_Rx *const rx, const Spw_Rx_Config *const config)
{
	(void)rx;

	SPW_PKTRX_SetDiscard(config->overrunningPacketPolicy ==
			     Spw_Rx_OverrunningPacketPolicy_Discard);

	SPW_PKTRX_InterruptEnable(config->irqMaskEnable);
	SPW_PKTRX_InterruptDisable(config->irqMaskDisable);
}

void Spw_Rx_reset(Spw_Rx *const rx)
{
	(void)rx;

	SPW_REGS->SPW_PKTRX1_SWRESET = SWRESET_ARM_PATTERN;
	// Generate read from SWRESET register (without it resets are unreliable)
	(void)SPW_REGS->SPW_PKTRX1_SWRESET;
	SPW_REGS->SPW_PKTRX1_SWRESET = SWRESET_TRIGGER_PATTERN;
}

uint32_t Spw_Rx_getIrq(const Spw_Rx *const rx)
{
	(void)rx;

	return SPW_REGS->SPW_PKTRX1_PI_R & SPW_PKTRX1_IRQREGS_ALLIRQ_MASK;
}

uint32_t Spw_Rx_getIrqMasked(const Spw_Rx *const rx)
{
	(void)rx;

	return SPW_REGS->SPW_PKTRX1_PI_RM & SPW_PKTRX1_IRQREGS_ALLIRQ_MASK;
}

uint32_t Spw_Rx_getAndClearIrq(Spw_Rx *const rx)
{
	(void)rx;

	uint32_t irq = SPW_REGS->SPW_PKTRX1_PI_R & SPW_PKTRX1_IRQREGS_ALLIRQ_MASK;
	SPW_REGS->SPW_PKTRX1_PI_C = irq;
	return irq;
}

uint32_t Spw_Rx_getAndClearIrqMasked(Spw_Rx *const rx)
{
	(void)rx;

	return SPW_PKTRX_IrqStatusGetMaskedAndClear() &
	       SPW_PKTRX1_IRQREGS_ALLIRQ_MASK;
}

void Spw_Rx_clearIrq(Spw_Rx *const rx)
{
	(void)rx;

	SPW_REGS->SPW_PKTRX1_PI_C = SPW_PKTRX1_IRQREGS_ALLIRQ_MASK;
}

void Spw_Rx_setNextRxBuffer(Spw_Rx *const rx,
			    const Spw_Rx_RxBufferConfig *const config)
{
	(void)rx;

	SPW_PKTRX_SetNextBuffer(config->rxDataAddress, config->rxDataLength,
				(SPW_PKTRX_INFO *)config->rxBufferAddress,
				config->rxBufferLength,
				config->isPacketSplitAndDeactivationEnabled,
				(SPW_PKTRX_NXTBUF_START)config->startCondition,
				config->startValue);
}

void Spw_Rx_abortOngoingPacketRx(Spw_Rx *const rx)
{
	(void)rx;

	SPW_REGS->SPW_PKTRX1_CURBUFCFG |= SPW_PKTRX1_CURBUFCFG_ABORT_Msk;
}

void Spw_Rx_getNextRxBufferConfig(const Spw_Rx *const rx,
				  Spw_Rx_RxBufferConfig *const config)
{
	(void)rx;

	uint32_t nxtBufCfg = SPW_REGS->SPW_PKTRX1_NXTBUFCFG;

	config->rxDataAddress = (uint8_t *)(uintptr_t)SPW_REGS->SPW_PKTRX1_NXTBUFDATAADDR;
	config->rxDataLength  = SPW_REGS->SPW_PKTRX1_NXTBUFDATALEN;
	config->rxBufferAddress = (Spw_Rx_RxBufferEntry *)(uintptr_t)SPW_REGS->SPW_PKTRX1_NXTBUFPKTADDR;
	config->rxBufferLength = (uint16_t)((nxtBufCfg &
					     SPW_PKTRX1_NXTBUFCFG_MAXCNT_Msk) >>
					    SPW_PKTRX1_NXTBUFCFG_MAXCNT_Pos);
	config->startValue     = (uint8_t)((nxtBufCfg &
					    SPW_PKTRX1_NXTBUFCFG_VALUE_Msk) >>
					   SPW_PKTRX1_NXTBUFCFG_VALUE_Pos);
	config->startCondition =
		(Spw_Rx_StartCondition)((nxtBufCfg &
					 SPW_PKTRX1_NXTBUFCFG_START_Msk) >>
					SPW_PKTRX1_NXTBUFCFG_START_Pos);
	config->isPacketSplitAndDeactivationEnabled =
		(nxtBufCfg & SPW_PKTRX1_NXTBUFCFG_SPLIT_Msk) != 0U;
}

void Spw_Rx_getStatus(const Spw_Rx *const rx, Spw_Rx_Status *const status)
{
	(void)rx;

	SPW_PKTRX_STATUS rawStatus = SPW_PKTRX_StatusGet();

	status->numOfPacketsInActiveBuffer =
		(uint16_t)((rawStatus & SPW_PKTRX_STATUS_COUNT_MASK) >>
			   SPW_PKTRX1_STATUS_COUNT_Pos);
	status->isIncomingPacketOngoing	      = (rawStatus &
						 SPW_PKTRX_STATUS_PACKET) != 0U;
	status->isPreviousReceiveBufferLocked = (rawStatus &
						 SPW_PKTRX_STATUS_LOCKED) != 0U;
	status->isNextReceiveBufferArmed = (rawStatus & SPW_PKTRX_STATUS_ARM) !=
					   0U;
	status->isCurrentReceiveBufferActive	 = (rawStatus &
						    SPW_PKTRX_STATUS_ACT) != 0U;
	status->isNextReceiveBufferPending	 = (rawStatus &
						    SPW_PKTRX_STATUS_PENDING) != 0U;
	status->isReceiveBufferReadyToDeactivate = (rawStatus &
						    SPW_PKTRX_STATUS_DEACT) != 0U;
}

void Spw_Rx_getPreviousRxBufferStatus(const Spw_Rx *const rx,
				      Spw_Rx_PreviousRxBufferStatus *const status)
{
	(void)rx;

	SPW_PKTRX_PREV_STATUS rawStatus = SPW_PKTRX_GetPreviousBufferStatus();

	status->receivedPackets = SPW_PKTRX_PREV_STATUS_GET_COUNT(rawStatus);
	status->wasEepReceived	= (rawStatus & SPW_PKTRX_PREV_STATUS_EEP) != 0U;
	status->isBufferInfoFull = (rawStatus & SPW_PKTRX_PREV_STATUS_FULLI) !=
				   0U;
	status->isBufferDataFull = (rawStatus & SPW_PKTRX_PREV_STATUS_FULLD) !=
				   0U;
	status->hasDmaErrorOccurred = (rawStatus &
				       SPW_PKTRX_PREV_STATUS_DMAERR) != 0U;
	status->isBufferLocked = (rawStatus & SPW_PKTRX_PREV_STATUS_LOCKED) != 0U;
}

void Spw_Rx_getRxBufferEntry(const Spw_Rx_RxBufferEntry *const entry,
			     Spw_Rx_RxBufferEntryStruct *const entryStruct)
{
	entryStruct->wasEepReceived    = entry->Eep != 0U;
	entryStruct->wasEopReceived    = entry->Eop != 0U;
	entryStruct->isPacketContinued = entry->Cont != 0U;
	entryStruct->isPacketSplit     = entry->Split != 0U;
	entryStruct->crc	       = (uint8_t)entry->Crc;
	entryStruct->dataAddress       = (uint8_t *)(uintptr_t)entry->DAddr;
	entryStruct->dataLength	       = (uint32_t)entry->DSize;
	entryStruct->packetTime	       = (uint32_t)entry->Etime;
}

void Spw_Rx_getStructuredIrq(const uint32_t rxIrqStatusValue,
			     Spw_Rx_IrqStatus *const status)
{
	status->receivedBufferActivatedIrqOccurred =
		(rxIrqStatusValue & (uint32_t)SPW_PKTRX_INT_MASK_ACT) != 0U;
	status->incomingPacketDiscardedIrqOccurred =
		(rxIrqStatusValue & (uint32_t)SPW_PKTRX_INT_MASK_DISCARD) != 0U;
	status->incomingEepDetectedIrqOccurred =
		(rxIrqStatusValue & (uint32_t)SPW_PKTRX_INT_MASK_EEP) != 0U;
	status->incomingEopDetectedIrqOccurred =
		(rxIrqStatusValue & (uint32_t)SPW_PKTRX_INT_MASK_EOP) != 0U;
	status->receivedBufferDeactivatedIrqOccurred =
		(rxIrqStatusValue & (uint32_t)SPW_PKTRX_INT_MASK_DEACT) != 0U;
}