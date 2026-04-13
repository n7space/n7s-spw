#ifndef TOOLCHAIN_SPECIFICS_H
#define TOOLCHAIN_SPECIFICS_H

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

#pragma GCC diagnostic push
#ifndef __cplusplus
   #pragma GCC diagnostic ignored "-Wnested-externs"
#endif
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wundef"
#include "cmsis_compiler.h"
#pragma GCC diagnostic pop

#include <sys/types.h>

#define NO_INIT        __attribute__((section(".no_init")))
#define SECTION(a)     __attribute__((__section__(a)))

#define CACHE_LINE_SIZE    (32u)
#define CACHE_ALIGN        __ALIGNED(CACHE_LINE_SIZE)

#define CACHE_ALIGNED_SIZE_GET(size)     ((size) + ((((size) % (CACHE_LINE_SIZE))!= 0U)? ((CACHE_LINE_SIZE) - ((size) % (CACHE_LINE_SIZE))) : (0U)))

#ifndef FORMAT_ATTRIBUTE
   #define FORMAT_ATTRIBUTE(archetype, string_index, first_to_check)  __attribute__ ((format (archetype, string_index, first_to_check)))
#endif


#ifdef __cplusplus
}
#endif

#endif

