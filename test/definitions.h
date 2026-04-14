#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "../arm-bsp/src/Matrix/Matrix.h"
#include "../arm-bsp/src/Nvic/Nvic.h"
#include "../arm-bsp/src/Tic/Tic.h"
#include "../arm-bsp/src/Wdt/Wdt.h"
#include "../src/link.h"
#include "../src/rx.h"
#include "../src/spw.h"
#include "../src/tx.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus

extern "C" {

#endif

void SYS_Initialize(void* data);

#ifdef __cplusplus
}
#endif

#endif /* DEFINITIONS_H */
