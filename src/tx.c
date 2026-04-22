/**@file
 * This file is part of the n7s-spw.
 *
 * @copyright 2018-2026 N7 Space Sp. z o.o.
 *
 * n7s-spw was developed under a programme of,
 * and funded by, the European Space Agency (the "ESA").
 *
 *
 * Licensed under the ESA Public License (ESA-PL) Permissive (Type 3),
 * Version 2.4 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://essr.esa.int/license/list
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tx.h"

#define SWRESET_ARM_PATTERN     0x4D616A6FU
#define SWRESET_TRIGGER_PATTERN 0x72546F6DU

#define SPW_PKTTX1_IRQREGS_ALLIRQ_MASK 0x0000000FU

void Spw_Tx_init(Spw_Tx* const tx)
{
  (void)tx;
}

void Spw_Tx_setConfig(Spw_Tx* const tx, const Spw_Tx_Config* const config)
{
  (void)tx;

  SPW_PKTTX_InterruptEnable(config->irqMaskEnable);
  SPW_PKTTX_InterruptDisable(config->irqMaskDisable);
}

void Spw_Tx_reset(const Spw_Tx* const tx)
{
  (void)tx;

  SPW_REGS->SPW_PKTTX1_SWRESET = SWRESET_ARM_PATTERN;
  // Generate read from SWRESET register (without it resets are unreliable)
  (void)SPW_REGS->SPW_PKTTX1_SWRESET;
  SPW_REGS->SPW_PKTTX1_SWRESET = SWRESET_TRIGGER_PATTERN;
}

void Spw_Tx_setNextSendList(Spw_Tx* const tx, const Spw_Tx_SendListConfig* const config)
{
  (void)tx;

  SPW_PKTTX_SetNextSendList((uint8_t*)config->routerByte,
    config->sendListAddress,
    config->sendListLength,
    config->abortOngoingSendListWhenStarted,
    (SPW_PKTTX_NXTSEND_START)config->sendCondition,
    config->startValue);
}

void Spw_Tx_setNextSendListEntry(Spw_Tx_SendListEntry* const entry,
  const Spw_Tx_SendListEntryStruct* const entryStruct)
{
  entry->Skip = entryStruct->isEntrySkipped ? 1U : 0U;
  entry->Entry = (uint32_t)entryStruct->entryType;
  entry->RSize = (uint32_t)entryStruct->routerByteLength;
  entry->Start = (uint32_t)entryStruct->startTime;
  entry->RB1 = (uint32_t)entryStruct->routerByte[0];
  entry->RB2 = (uint32_t)entryStruct->routerByte[1];
  entry->RB3 = (uint32_t)entryStruct->routerByte[2];
  entry->RB4 = (uint32_t)entryStruct->routerByte[3];
  entry->RB5 = (uint32_t)entryStruct->routerByte[4];
  entry->RB6 = (uint32_t)entryStruct->routerByte[5];
  entry->RB7 = (uint32_t)entryStruct->routerByte[6];
  entry->RB8 = (uint32_t)entryStruct->routerByte[7];
  entry->EscMask = (uint32_t)entryStruct->escapeCharMask;
  entry->EscChar = (uint32_t)entryStruct->escapeChar;
  entry->HCrc = entryStruct->calculateHeaderCrc ? 1U : 0U;
  entry->HSize = (uint32_t)entryStruct->headerSize;
  entry->HAddr = (uint32_t)(uintptr_t)entryStruct->headerAddress;
  entry->DCrc = entryStruct->calculateDataCrc ? 1U : 0U;
  entry->DSize = (uint32_t)entryStruct->dataSize;
  entry->DAddr = (uint32_t)(uintptr_t)entryStruct->dataAddress;
  entry->TimeOut = (uint32_t)entryStruct->timeout;
}

void Spw_Tx_abortCurrentSendList(Spw_Tx* const tx)
{
  (void)tx;

  SPW_REGS->SPW_PKTTX1_CURSENDCFG |= SPW_PKTTX1_CURSENDCFG_ABORT_Msk;
}

void Spw_Tx_getCurrentSendListConfig(const Spw_Tx* const tx, Spw_Tx_SendListConfig* const config)
{
  (void)tx;

  uint32_t curSendCfg = SPW_REGS->SPW_PKTTX1_CURSENDCFG;
  uint32_t curSendRout = SPW_REGS->SPW_PKTTX1_CURSENDROUT;

  config->sendListAddress = (Spw_Tx_SendListEntry*)(uintptr_t)SPW_REGS->SPW_PKTTX1_CURSENDADDR;
  config->sendListLength
    = (uint16_t)((curSendCfg & SPW_PKTTX1_CURSENDCFG_LEN_Msk) >> SPW_PKTTX1_CURSENDCFG_LEN_Pos);
  config->routerByte[0] = (uint8_t)((curSendRout & SPW_PKTTX1_CURSENDROUT_BYTE1_Msk)
                                    >> SPW_PKTTX1_CURSENDROUT_BYTE1_Pos);
  config->routerByte[1] = (uint8_t)((curSendRout & SPW_PKTTX1_CURSENDROUT_BYTE2_Msk)
                                    >> SPW_PKTTX1_CURSENDROUT_BYTE2_Pos);
  config->routerByte[2] = (uint8_t)((curSendRout & SPW_PKTTX1_CURSENDROUT_BYTE3_Msk)
                                    >> SPW_PKTTX1_CURSENDROUT_BYTE3_Pos);
  config->routerByte[3] = (uint8_t)((curSendRout & SPW_PKTTX1_CURSENDROUT_BYTE4_Msk)
                                    >> SPW_PKTTX1_CURSENDROUT_BYTE4_Pos);
  // sendCondition, startValue, abortOngoingSendListWhenStarted are not
  // available in current send list registers; set to defaults.
  config->sendCondition = (Spw_Tx_SendCondition)0;
  config->startValue = 0U;
  config->abortOngoingSendListWhenStarted = false;
}

void Spw_Tx_getSendListEntry(const Spw_Tx_SendListEntry* const entry,
  Spw_Tx_SendListEntryStruct* const entryStruct)
{
  entryStruct->isEntrySkipped = entry->Skip != 0U;
  entryStruct->entryType = (Spw_Tx_EntryType)entry->Entry;
  entryStruct->routerByteLength = (uint8_t)entry->RSize;
  entryStruct->startTime = (uint32_t)entry->Start;
  entryStruct->routerByte[0] = (uint8_t)entry->RB1;
  entryStruct->routerByte[1] = (uint8_t)entry->RB2;
  entryStruct->routerByte[2] = (uint8_t)entry->RB3;
  entryStruct->routerByte[3] = (uint8_t)entry->RB4;
  entryStruct->routerByte[4] = (uint8_t)entry->RB5;
  entryStruct->routerByte[5] = (uint8_t)entry->RB6;
  entryStruct->routerByte[6] = (uint8_t)entry->RB7;
  entryStruct->routerByte[7] = (uint8_t)entry->RB8;
  entryStruct->escapeCharMask = (uint8_t)entry->EscMask;
  entryStruct->escapeChar = (uint8_t)entry->EscChar;
  entryStruct->calculateHeaderCrc = entry->HCrc != 0U;
  entryStruct->headerSize = (uint8_t)entry->HSize;
  entryStruct->headerAddress = (uint8_t*)(uintptr_t)entry->HAddr;
  entryStruct->calculateDataCrc = entry->DCrc != 0U;
  entryStruct->dataSize = (uint32_t)entry->DSize;
  entryStruct->dataAddress = (uint8_t*)(uintptr_t)entry->DAddr;
  entryStruct->timeout = (uint32_t)entry->TimeOut;
}

void Spw_Tx_setIrqMask(Spw_Tx* const tx, const uint32_t mask)
{
  (void)tx;

  SPW_REGS->SPW_PKTTX1_IM = mask & SPW_PKTTX1_IRQREGS_ALLIRQ_MASK;
}

uint32_t Spw_Tx_getIrq(const Spw_Tx* const tx)
{
  (void)tx;

  return SPW_REGS->SPW_PKTTX1_PI_R & SPW_PKTTX1_IRQREGS_ALLIRQ_MASK;
}

uint32_t Spw_Tx_getIrqMasked(const Spw_Tx* const tx)
{
  (void)tx;

  return SPW_REGS->SPW_PKTTX1_PI_RM & SPW_PKTTX1_IRQREGS_ALLIRQ_MASK;
}

uint32_t Spw_Tx_getAndClearIrq(const Spw_Tx* const tx)
{
  (void)tx;

  uint32_t irq = SPW_REGS->SPW_PKTTX1_PI_R & SPW_PKTTX1_IRQREGS_ALLIRQ_MASK;
  SPW_REGS->SPW_PKTTX1_PI_C = irq;
  return irq;
}

uint32_t Spw_Tx_getAndClearIrqMasked(const Spw_Tx* const tx)
{
  (void)tx;

  return SPW_PKTTX_IrqStatusGetMaskedAndClear() & SPW_PKTTX1_IRQREGS_ALLIRQ_MASK;
}

void Spw_Tx_clearIrq(const Spw_Tx* const tx)
{
  (void)tx;

  SPW_REGS->SPW_PKTTX1_PI_C = SPW_PKTTX1_IRQREGS_ALLIRQ_MASK;
}

void Spw_Tx_getStructuredIrq(const uint32_t linkIrqStatusValue, Spw_Tx_IrqStatus* const status)
{
  status->eepSentIrqOccurred = (linkIrqStatusValue & (uint32_t)SPW_PKTTX_INT_MASK_EEP) != 0U;
  status->eopSentIrqOccurred = (linkIrqStatusValue & (uint32_t)SPW_PKTTX_INT_MASK_EOP) != 0U;
  status->sendListActivatedIrqOccurred
    = (linkIrqStatusValue & (uint32_t)SPW_PKTTX_INT_MASK_ACT) != 0U;
  status->sendListDeactivatedIrqOccurred
    = (linkIrqStatusValue & (uint32_t)SPW_PKTTX_INT_MASK_DEACT) != 0U;
}

void Spw_Tx_getStatus(Spw_Tx* const tx, Spw_Tx_Status* const status)
{
  (void)tx;

  SPW_PKTTX_STATUS rawStatus = SPW_PKTTX_StatusGet();

  status->lastState = (Spw_Tx_DeactivatedSendListState)SPW_PKTTX_STATUS_PREV_GET(rawStatus);
  status->isReadyToDeactivateCurrentSendList = (rawStatus & SPW_PKTTX_STATUS_DEACT) != 0U;
  status->isNextSendListPending = (rawStatus & SPW_PKTTX_STATUS_PENDING) != 0U;
  status->isCurrentSendListActive = (rawStatus & SPW_PKTTX_STATUS_ACT) != 0U;
  status->isNextSendListArmed = (rawStatus & SPW_PKTTX_STATUS_ARM) != 0U;
}
