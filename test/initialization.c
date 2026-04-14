#include "definitions.h"
#include "device.h"
#include "interrupts.h"

Spw g_spw;

void SYS_Initialize(void* data)
{
  (void)data;

  // 1. Disable watchdog.
  Wdt wdt;
  Wdt_init(&wdt);
  const Wdt_Config wdtCfg = {
    .isDisabled = true,
    .counterValue = 0u,
    .deltaValue = 0u,
    .isResetEnabled = false,
    .doesFaultActivateProcessorReset = false,
    .isFaultInterruptEnabled = false,
    .isHaltedOnIdle = false,
    .isHaltedOnDebug = false,
  };
  Wdt_setConfig(&wdt, &wdtCfg);

  // 2. Disable reinforced safety watchdog directly
  RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk;

  // 3. Configure Matrix
  // Workaround for Hardware bug related to memory access on SAMRH71F20
  // impacts peripherals using DMA: Mcan, Xdmac, Gmac and SpaceWire
  // details in document DS80000875D - Rad-Hard 32-bit Arm Cortex-M7 Microcontroller
  // for Aerospace Applications Errata Sheet
  // Erratum number 11
  Matrix matrix;
  Matrix_init(&matrix, Matrix_getDeviceBaseAddress());

  const Matrix_Slave flexramSlaves[] = {
    Matrix_Slave_Flexram0,
    Matrix_Slave_Flexram1,
    Matrix_Slave_Flexram2,
  };
  const Matrix_SlaveRegionProtectionConfig config = {
    .isPrivilegedRegionUserWriteAllowed = true,
    .isPrivilegedRegionUserReadAllowed = true,
    .regionSplitOffset = Matrix_Size_128MB,
    .regionOrder = Matrix_RegionSplitOrder_UpperPrivilegedLowerUser,
  };

  for (uint32_t i = 0; i < 3; i++)
  {
    for (uint32_t j = 0; j < (uint32_t)Matrix_ProtectedRegionId_Count; j++)
    {
      Matrix_setSlaveRegionProtectionConfig(
        &matrix, flexramSlaves[i], (Matrix_ProtectedRegionId)j, &config);
    }
  }

  // 4. SpaceWire driver init. Divisors values from microchip example test.
  Spw_init(&g_spw);
  const Spw_Config spwCfg = {
        .link = {
            [0] = {
                .txInitDiv               = 19U,
                .txOperDiv               = 0U,
                .command                 = 3U,
                .escChar                 = 0U,
                .escEvent1               = { .active = false, .mask = 0U, .value = 0U },
                .escEvent2               = { .active = false, .mask = 0U, .value = 0U },
                .distributedIrqToEnable  = 0U,
                .distributedIrqToDisable = 0U,
                .distributedAckIrqToEnable  = 0U,
                .distributedAckIrqToDisable = 0U,
            },
            [1] = {
                .txInitDiv               = 19U,
                .txOperDiv               = 0U,
                .command                 = 2U,
                .escChar                 = 0U,
                .escEvent1               = { .active = false, .mask = 0U, .value = 0U },
                .escEvent2               = { .active = false, .mask = 0U, .value = 0U },
                .distributedIrqToEnable  = 0U,
                .distributedIrqToDisable = 0U,
                .distributedAckIrqToEnable  = 0U,
                .distributedAckIrqToDisable = 0U,
            },
        },
        .rx = {
            .overrunningPacketPolicy = Spw_Rx_OverrunningPacketPolicy_Stall,
            .irqMaskEnable           = 0U,
            .irqMaskDisable          = 0U,
        },
        .tx = {
            .irqMaskEnable  = 0U,
            .irqMaskDisable = 0U,
        },
    };
  Spw_setConfig(&g_spw, &spwCfg);

  // 6. NVIC – enable SPW interrupt lines and global IRQs.
  Nvic_enableInterrupt(Nvic_Irq_Spw_Irq0);
  Nvic_enableInterrupt(Nvic_Irq_Spw_Irq1);
  Nvic_enableIrq();
}

void SPW_InterruptHandler(void)
{
  Spw_handleInterrupt(&g_spw);
}
