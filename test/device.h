#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <stddef.h>

#define _UINT32_(x)   ((uint32_t)(x))

#define __I   volatile const  /* read-only  */
#define __O   volatile        /* write-only */
#define __IO  volatile        /* read/write */

#include "microchip_spw/spw.h"

#define SPW_BASE_ADDRESS  (0x40040000u)
#define SPW_REGS          ((spw_registers_t *)SPW_BASE_ADDRESS)

typedef struct {
    __IO uint32_t RSWDT_MR;  // +0x00 Mode Register
    __IO uint32_t RSWDT_CR;  // +0x04 Control Register
    __I  uint32_t RSWDT_SR;  // +0x08 Status Register
} Rswdt_Regs_t;

#define RSWDT_BASE_ADDRESS  (0x40100010u)
#define RSWDT_REGS          ((Rswdt_Regs_t *)RSWDT_BASE_ADDRESS)
#define RSWDT_MR_WDDIS_Msk  (0x00008000u)   // Watchdog Disable

#define FLEXRAM_ADDR  (0x21000000UL)
#define FlexRAM_SIZE  (0x00080000UL)  // 512 KB – matches LENGTH(ram) in LD

#endif /* DEVICE_H */
