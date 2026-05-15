// Stub for Microchip device.h

#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>
#include <stddef.h>

#define _UINT32_(x)   ((uint32_t)(x))

#define __I   volatile const  /* read-only  */
#define __O   volatile        /* write-only */
#define __IO  volatile        /* read/write */

#include "microchip_spw/spw.h"

/* Peripheral base pointer */
#define SPW_BASE_ADDRESS (0x40040000u)
#define SPW_REGS         ((spw_registers_t*)SPW_BASE_ADDRESS)

#endif
