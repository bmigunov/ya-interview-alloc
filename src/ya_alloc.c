/*
 * Custom pool memory allocation implementation.
 */


#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "FreeRTOS.h"
#include "task.h"

#include "ya_alloc.h"




static uint8_t pool[POOL_SIZE];
static uint8_t map[MAP_SIZE];


void *ya_alloc(void)
{
    void *allocated_block = NULL;

    vTaskSuspendAll();

    for (uint32_t i = 0; i < MAP_SIZE; ++i)
    {
        for (uint32_t j = 0; j < 8; ++j)
        {
            if ((map[i] & (0x01 << j)) == 0)
            {
                uint32_t block_index = 0;

                map[i] |= (0x01 << j);
                block_index = (BLOCK_SIZE * (i * sizeof(uint8_t)) + BLOCK_SIZE * j);
                allocated_block = (void *)(pool + block_index);

                break;
            }
        }

        if (allocated_block)
            break;
    }

    xTaskResumeAll();

    return allocated_block;
};

void ya_free(void *block_addr)
{
#ifdef ARCH64
    uint32_t offset = ((uint64_t)block_addr - (uint64_t)pool) / BLOCK_SIZE;
#else
    uint32_t offset = ((uint32_t)block_addr - (uint32_t)pool) / BLOCK_SIZE;
#endif
    uint32_t map_index = offset / 8;
    uint32_t bit_offset = offset % 8;

    vTaskSuspendAll();

// Out of bounds
    if ((offset < 0) || (offset >= BLOCK_SIZE))
        exit(EFAULT);

// Unaligned address
#ifdef ARCH64
    if (((uint64_t)block_addr - (uint64_t)pool) % BLOCK_SIZE != 0)
#else
    if (((uint32_t)block_addr - (uint32_t)pool) % BLOCK_SIZE != 0)
#endif
        exit(EFAULT);

    map[map_index] &= (~(0x01 << bit_offset));

    xTaskResumeAll();

    return;
};

void ya_heap_reset(void)
{
    for (uint32_t i = 0; i < BLOCKS_TOTAL; ++i)
        map[i] = 0;

    return;
};
