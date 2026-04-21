#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "device.h"
#include <spw.h>

void SYS_Initialize(void* data);

#define SPW_PKTRX_ROUTER_PORT 9U // loopback destination

#define NUM_PACKETS   256U
#define PACKET_SIZE   4U
#define RX_PACKET_NUM NUM_PACKETS
#define RX_BUF_SIZE   (RX_PACKET_NUM * PACKET_SIZE)

static uint8_t __attribute__((aligned(32))) tx_data[NUM_PACKETS][PACKET_SIZE];

static Spw_Tx_SendListEntry __attribute__((aligned(32))) tx_send_list[NUM_PACKETS];

static uint8_t __attribute__((aligned(32))) rx_buf[RX_BUF_SIZE];

static Spw_Rx_RxBufferEntry __attribute__((aligned(32))) rx_info[RX_PACKET_NUM];

static volatile bool tx_done = false;
static volatile bool rx_done = false;

static volatile bool test_result = false;

static void spw_rx_callback(void* arg, const Spw_Rx_IrqStatus* const irqStatus)
{
  (void)arg;
  if (irqStatus->receivedBufferDeactivatedIrqOccurred)
  {
    rx_done = true;
  }
}

static void spw_tx_callback(void* arg, const Spw_Tx_IrqStatus* const irqStatus)
{
  (void)arg;
  if (irqStatus->sendListDeactivatedIrqOccurred)
  {
    tx_done = true;
  }
}

int main(void)
{
  SYS_Initialize(NULL);

  extern Spw g_spw;

  // Prepare handlers, enable irqs
  const Spw_TxHandler txHandler = { .callback = spw_tx_callback, .arg = NULL };
  const Spw_RxHandler rxHandler = { .callback = spw_rx_callback, .arg = NULL };
  Spw_registerTxHandler(&g_spw, txHandler);
  Spw_registerRxHandler(&g_spw, rxHandler);

  const Spw_Tx_Config txCfg = {
    .irqMaskEnable = (uint32_t)Spw_Tx_Irq_SendListDeactivated,
    .irqMaskDisable = 0U,
  };
  const Spw_Rx_Config rxCfg = {
    .overrunningPacketPolicy = Spw_Rx_OverrunningPacketPolicy_Stall,
    .irqMaskEnable = (uint32_t)Spw_Rx_Irq_ReceivedBufferDeactivated,
    .irqMaskDisable = 0U,
  };
  Spw_Tx_setConfig(&g_spw.tx, &txCfg);
  Spw_Rx_setConfig(&g_spw.rx, &rxCfg);

  // Prepare TX send list: 256 packets, each 4 bytes, routed to the
  // internal PKTRX port via SPW_ROUTER_PKTRX_PORT (loopback).
  // Each packet has data in form of counter value (0, 1, 2, ..., 255)
  for (uint32_t i = 0U; i < NUM_PACKETS; i++)
  {
    tx_data[i][0] = (uint8_t)(i & 0xFFU);
    tx_data[i][1] = 0U;
    tx_data[i][2] = 0U;
    tx_data[i][3] = 0U;

    const Spw_Tx_SendListEntryStruct entry = {
      .isEntrySkipped = false,
      .entryType = Spw_Tx_EntryType_PacketData,
      .routerByteLength = 1U,
      .routerByte = { SPW_PKTRX_ROUTER_PORT, 0, 0, 0, 0, 0, 0, 0 },
      .startTime = 0U,
      .escapeCharMask = 0xFU,
      .escapeChar = 0xFAU,
      .calculateHeaderCrc = false,
      .headerSize = 0U,
      .headerAddress = NULL,
      .calculateDataCrc = false,
      .dataSize = PACKET_SIZE,
      .dataAddress = tx_data[i],
      .timeout = 0U,
    };
    Spw_Tx_setNextSendListEntry(&tx_send_list[i], &entry);
  }

  // Prepare RX buffer list: 256 entries, each with a 4-byte buffer.
  const Spw_Rx_RxBufferConfig rxBufCfg = {
    .isPacketSplitAndDeactivationEnabled = false,
    .startCondition = Spw_Rx_StartCondition_StartNow,
    .startValue = 0U,
    .rxBufferAddress = rx_info,
    .rxBufferLength = RX_PACKET_NUM,
    .rxDataAddress = rx_buf,
    .rxDataLength = RX_BUF_SIZE,
  };
  Spw_Rx_setNextRxBuffer(&g_spw.rx, &rxBufCfg);

  // Wait for RX buffer to become active before starting TX.
  {
    Spw_Rx_Status rxStatus;
    do
    {
      Spw_Rx_getStatus(&g_spw.rx, &rxStatus);
    } while (!rxStatus.isCurrentReceiveBufferActive);
  }

  // Start transmission
  const Spw_Tx_SendListConfig txListCfg = {
    .sendCondition = Spw_Tx_SendCondition_StartNow,
    .sendListLength = NUM_PACKETS,
    .sendListAddress = tx_send_list,
    .routerByte = { 0U, 0U, 0U, 0U },
    .abortOngoingSendListWhenStarted = false,
    .startValue = 0U,
  };
  Spw_Tx_setNextSendList(&g_spw.tx, &txListCfg);

  // Wait for TX to complete.
  volatile uint32_t tx_timeout = 2000000U;
  while (!tx_done && (tx_timeout-- > 0U))
  {
  }

  // Wait for RX to complete.
  volatile uint32_t rx_timeout = 2000000U;
  while (!rx_done && (rx_timeout-- > 0U))
  {
  }

  if (!rx_done)
  {
    // Fallback, force abort so we can finish test
    Spw_Rx_abortOngoingPacketRx(&g_spw.rx);
    while (!rx_done)
    {
    }
  }

  // Verify results
  Spw_Rx_PreviousRxBufferStatus prevStatus;
  Spw_Rx_getPreviousRxBufferStatus(&g_spw.rx, &prevStatus);

  if (!prevStatus.isBufferLocked)
  {
    test_result = false;
  }
  else if (prevStatus.receivedPackets != NUM_PACKETS)
  {
    test_result = false;
  }
  else
  {
    test_result = true;
    // check received packets, packet data should be (0, 1, 2, ..., 255)
    for (uint16_t i = 0; i < NUM_PACKETS; i++)
    {
      Spw_Rx_RxBufferEntryStruct entry;
      Spw_Rx_getRxBufferEntry(&rx_info[i], &entry);

      if (entry.dataLength != PACKET_SIZE)
      {
        test_result = false;
        break;
      }

      const uint8_t* const rx_buf_end = rx_buf + RX_BUF_SIZE;
      if ((entry.dataAddress < rx_buf) || (entry.dataAddress + entry.dataLength > rx_buf_end))
      {
        test_result = false;
        break;
      }

      const uint8_t* data = entry.dataAddress;
      if (data[0] != (uint8_t)(i & 0xFFU))
      {
        test_result = false;
        break;
      }
    }
  }

  // debug check test_result
  while (true)
  {
  }

  return 0;
}
