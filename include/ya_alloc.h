#ifndef YA_ALLOC_H
#define YA_ALLOC_H

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_POOL_SIZE  (2 * 1024)
#define DEFAULT_BLOCK_SIZE 8

#ifndef POOL_SIZE

#define POOL_SIZE DEFAULT_POOL_SIZE

#endif /* POOL_SIZE */

#ifndef BLOCK_SIZE

#define BLOCK_SIZE DEFAULT_BLOCK_SIZE

#endif /* BLOCK_SIZE */

#if POOL_SIZE % BLOCK_SIZE != 0
#error
#endif

#define BLOCKS_TOTAL (POOL_SIZE / BLOCK_SIZE)
#define MAP_SIZE     (BLOCKS_TOTAL / 8)




void *ya_alloc(void);
void ya_free(void *);
void ya_heap_reset(void);

#endif /* YA_ALLOC_H */
