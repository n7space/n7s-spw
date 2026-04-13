#include "definitions.h"
#include "device.h"
#include "interrupts.h"

static Spw  g_spw;
static Mpu  g_mpu;
static Wdt  g_wdt;

void SYS_Initialize ( void* data )
{
    (void)data;

    // 1. Disable watchdog as early as possible.
    Wdt_init(&g_wdt);
    const Wdt_Config wdtCfg = {
        .isDisabled                      = true,
        .counterValue                    = 0u,
        .deltaValue                      = 0u,
        .isResetEnabled                  = false,
        .doesFaultActivateProcessorReset = false,
        .isFaultInterruptEnabled         = false,
        .isHaltedOnIdle                  = false,
        .isHaltedOnDebug                 = false,
    };
    Wdt_setConfig(&g_wdt, &wdtCfg);

    // 2. Disable reinforced safety watchdog directly via RSWDT registers, as the Wdt driver does not support it.
    RSWDT_REGS->RSWDT_MR = RSWDT_MR_WDDIS_Msk;

    // 3. Bus Matrix – initialise with reset-state defaults
    Matrix matrix;
    Matrix_init(&matrix, Matrix_getDeviceBaseAddress());

    // 4. MPU – enable with default memory map for privileged access.
    Mpu_init(&g_mpu);
    const Mpu_Config mpuCfg = {
        .isEnabled                 = true,
        .isDefaultMemoryMapEnabled = true,
        .isMpuEnabledInHandlers    = false,
    };
    Mpu_setConfig(&g_mpu, &mpuCfg);

    // 5. SpaceWire driver init.
    Spw_init(&g_spw);

    // 6. NVIC – enable SPW interrupt lines and global IRQs.
    Nvic_enableInterrupt(Nvic_Irq_Spw_Irq0);
    Nvic_enableInterrupt(Nvic_Irq_Spw_Irq1);
    Nvic_enableIrq();
}

/* SPW interrupt handler - dispatches to the SPW driver */
void SPW_InterruptHandler(void)
{
    Spw_handleInterrupt(&g_spw);
}
