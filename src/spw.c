// N7 Space Sp. z o.o.
// https://n7space.com
// COPYRIGHT (C) 2022-2025

#include "spw.h"

void Spw_init(Spw* const spw)
{
  Spw_Link_init(&spw->link[0], SPW_LINK_1);
  Spw_Link_init(&spw->link[1], SPW_LINK_2);
  Spw_Rx_init(&spw->rx);
  Spw_Tx_init(&spw->tx);

  spw->txHandler.callback = NULL;
  spw->txHandler.arg = NULL;
  spw->rxHandler.callback = NULL;
  spw->rxHandler.arg = NULL;
}

void Spw_setConfig(Spw* const spw, const Spw_Config* const config)
{
  Spw_Link_setConfig(&spw->link[0], &config->link[0]);
  Spw_Link_setConfig(&spw->link[1], &config->link[1]);
  Spw_Rx_setConfig(&spw->rx, &config->rx);
  Spw_Tx_setConfig(&spw->tx, &config->tx);
}

void Spw_registerTxHandler(Spw* const spw, const Spw_TxHandler handler)
{
  spw->txHandler = handler;
}

void Spw_registerRxHandler(Spw* const spw, const Spw_RxHandler handler)
{
  spw->rxHandler = handler;
}

void Spw_handleInterrupt(Spw* const spw)
{
  uint32_t txIrq = Spw_Tx_getAndClearIrqMasked(&spw->tx);
  if ((txIrq != 0U) && (spw->txHandler.callback != NULL))
  {
    Spw_Tx_IrqStatus txStatus;
    Spw_Tx_getStructuredIrq(txIrq, &txStatus);
    spw->txHandler.callback(spw->txHandler.arg, &txStatus);
  }

  uint32_t rxIrq = Spw_Rx_getAndClearIrqMasked(&spw->rx);
  if ((rxIrq != 0U) && (spw->rxHandler.callback != NULL))
  {
    Spw_Rx_IrqStatus rxStatus;
    Spw_Rx_getStructuredIrq(rxIrq, &rxStatus);
    spw->rxHandler.callback(spw->rxHandler.arg, &rxStatus);
  }
}
