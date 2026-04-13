#include "definitions.h"

volatile uint32_t counter = 0U;

int main(void)
{
    SYS_Initialize(NULL);

    while (1)
    {
        ++counter;
    }

    return 0;
}
