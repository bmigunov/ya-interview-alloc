#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"

#include "ya_alloc.h"

#ifdef BUILD_DIR
#define BUILD BUILD_DIR
#else
#define BUILD "./"
#endif /* BUILD_DIR */




static void ya_alloc_test_task(void *);

int main(void)
{
    printf("Testing 'ya_malloc' library\n");

    xTaskCreate(ya_alloc_test_task, "ya_alloc_test_task",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    return 0;
};

void ya_alloc_test_task(void *params)
{
    void *allocated_block[3] = {NULL};
    void *ptr = NULL;

    printf("POOL_SIZE: %d bytes; BLOCK_SIZE: %d bytes; BLOCKS_TOTAL: %d; "
           "MAP_SIZE: %d bytes\n", POOL_SIZE, BLOCK_SIZE, BLOCKS_TOTAL,
           MAP_SIZE);

    printf("Trying to allocate all available memory\n");
    for(uint32_t i = 0; ; ++i)
    {
        allocated_block[0] = ya_alloc();
        if (allocated_block[0] == NULL)
        {
            if (i == BLOCKS_TOTAL)
                printf("Out of memory\nIteration %d is equal to the number of "
                       "available blocks.\nPASS\n\n", i);
            else
            {
                printf("Out of memory\nIteration %d is not equal to the number"
                       "of available blocks.\nFAIL\n\n", i);
                exit(EFAULT);
            }
            break;
        }
    }

    printf("Resetting allocated heap\n");
    ya_heap_reset();

    printf("Testing 'ya_free' routine\nAllocating 3 blocks of memory, "
           "deallocating the 2nd one and allocating memory once again.\nThe "
           "address of the allocated block should be the same as the address "
           "of the 2nd one (both lasly freed & first free block in the "
           "pool)\n");
    for (uint32_t i = 0; i < 3; ++i)
    {
        allocated_block[i] = ya_alloc();
    }
    ya_free(allocated_block[1]);
    ptr = ya_alloc();
    if (ptr != allocated_block[1])
    {
        printf("FAIL\n\n");
        exit(EFAULT);
    }
    else
        printf("PASS\n\n");

    return;
};

void vApplicationDaemonTaskStartupHook(void)
{
    return;
}
