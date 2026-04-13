

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "../src/spw.h"
#include "../src/link.h"
#include "../src/rx.h"
#include "../src/tx.h"
#include "../arm-bsp/src/Matrix/Matrix.h"
#include "../arm-bsp/src/Nvic/Nvic.h"
#include "../arm-bsp/src/Mpu/Mpu.h"
#include "../arm-bsp/src/Tic/Tic.h"
#include "../arm-bsp/src/Wdt/Wdt.h"

#ifdef __cplusplus

extern "C" {

#endif

void SYS_Initialize( void *data );

#ifdef __cplusplus
}
#endif

#endif /* DEFINITIONS_H */

