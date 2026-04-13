#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void _exit(int status);

void _exit(int status)
{
#ifdef __DEBUG
    __BKPT(0);
#endif

    while (true)
    {
    }
}

void __attribute__((weak)) _init(void) {}
void __attribute__((weak)) _fini(void) {}

#ifdef __cplusplus
}
#endif
