/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <stdint.h>

#include "sram_heap.h"
#include "ameba_soc.h"
#include "FreeRTOS.h"

#define BYTE_ALIGNMENT 		    (portBYTE_ALIGNMENT)

/* NOTE: struct size must be a 2's power */
typedef struct _sramMemChunk {
	struct _sramMemChunk *next;
	int size;
} sramMemChunk;

typedef struct sram_Heap {
	struct _sramMemChunk *FreeList;
} sram_Heap;

typedef sramMemChunk sram_heap_buf_t;

/**
 * Utility macro to allocate a heap of size size.
 * @param name Variable name for the heap.
 * @param size Heap size in bytes.
 */
#define sram_ROUND_UP2(x, pad) (((x) + ((BYTE_ALIGNMENT) - 1)) & ~((BYTE_ALIGNMENT) - 1))
#define HEAP_STRUCT_SIZE (((sizeof(sramMemChunk) + (unsigned int)BYTE_ALIGNMENT - 1) & ~(((unsigned int)BYTE_ALIGNMENT) - 1)))

static void sram_heap_init(void);
static void *sram_heap_allocmem(int size);
static void sram_reserved_heap_freemem(void *mem, int size);

static struct sram_Heap g_sram_heap;
static int g_sram_heap_inited = 0;
extern char _memmap_seg_sram_dsp_end;  // Address after .bss section, defined in <lsp_dir>/ldscripts/elf32xtensa.x
extern char _memmap_seg_sram_dsp_max;  // psram max address

static void sram_heap_init(void)
{
	if (&_memmap_seg_sram_dsp_max <=  &_memmap_seg_sram_dsp_end) {
		DiagPrintf("sram invalid, please check RTK_LSP\n");
		while (1);
	} else {
		DiagPrintf("SRAM valid heap size: %d Bytes. \n", &_memmap_seg_sram_dsp_max - &_memmap_seg_sram_dsp_end);
	}

	/* Initialize heap with a single big chunk */
	g_sram_heap.FreeList = (sramMemChunk *)(&_memmap_seg_sram_dsp_end);
	g_sram_heap.FreeList->next = NULL;
	g_sram_heap.FreeList->size = (int)(&_memmap_seg_sram_dsp_max - &_memmap_seg_sram_dsp_end);

	g_sram_heap_inited = 1;
}

/**
  * @brief  allocate free memory from sram.
  * @param  size: size of the memory you want.
  * @retval: header address of the memory allocated.
  */
static void *sram_heap_allocmem(int size)
{
	sramMemChunk *chunk, *prev;
	struct sram_Heap *h = &g_sram_heap;

	vTaskSuspendAll();

	if (!g_sram_heap_inited)	{
		sram_heap_init();
	}

	/* Round size up to the allocation granularity */
	size = (int) sram_ROUND_UP2(size, (int) sizeof(sramMemChunk));

	/* Handle allocations of 0 bytes */
	if (!size) {
		size = sizeof(sramMemChunk);
	}

	/* Walk on the free list looking for any chunk big enough to
	 * fit the requested block size.
	 */
	for (prev = (sramMemChunk *)&h->FreeList, chunk = h->FreeList;
		 chunk;
		 prev = chunk, chunk = chunk->next) {
		if (chunk->size >= size) {
			if (chunk->size == size) {
				/* Just remove this chunk from the free list */
				prev->next = chunk->next;
				xTaskResumeAll();
				return (void *)chunk;
			} else {
				/* Allocate from the END of an existing chunk */
				chunk->size -= size;
				void *result = (void *)((uint8_t *)chunk + chunk->size);
				xTaskResumeAll();
				return result;
			}
		}
	}
	xTaskResumeAll();
	return NULL; /* fail */
}

/**
  * @brief  free memory in sram.
  * @param  mem: header address of the memory to be freed.
  * @param  size: size of the memory.
  * @retval none
  */
static void sram_reserved_heap_freemem(void *mem, int size)
{
	sramMemChunk *prev;
	struct sram_Heap *h = &g_sram_heap;

	vTaskSuspendAll();

	if (!g_sram_heap_inited)	{
		sram_heap_init();
	}

	/* Round size up to the allocation granularity */
	size = (int) sram_ROUND_UP2(size, (int) sizeof(sramMemChunk));

	/* Handle allocations of 0 bytes */
	if (!size) {
		size = sizeof(sramMemChunk);
	}

	/* Special cases: first chunk in the free list or memory completely full */
	if (((uint8_t *)mem) < ((uint8_t *)h->FreeList) || !h->FreeList) {
		/* Insert memory block before the current free list head */
		prev = (sramMemChunk *)mem;
		prev->next = h->FreeList;
		prev->size = size;
		h->FreeList = prev;
	} else { /* Normal case: not the first chunk in the free list */
		/*
		 * Walk on the free list. Stop at the insertion point (when mem
		 * is between prev and prev->next)
		 */
		prev = h->FreeList;
		while (prev->next < (sramMemChunk *)mem && prev->next) {
			prev = prev->next;
		}

		/* Should it be merged with previous block? */
		if (((uint8_t *)prev) + prev->size == ((uint8_t *)mem)) {
			/* Yes */
			prev->size += size;
		} else { /* not merged with previous chunk */
			sramMemChunk *curr = (sramMemChunk *)mem;

			/* insert it after the previous node
			 * and move the 'prev' pointer forward
			 * for the following operations
			 */
			curr->next = prev->next;
			curr->size = size;
			prev->next = curr;

			/* Adjust for the following test */
			prev = curr;
		}
	}

	/* Also merge with next chunk? */
	if (((uint8_t *)prev) + prev->size == ((uint8_t *)prev->next)) {
		prev->size += prev->next->size;
		prev->next = prev->next->next;
	}

	xTaskResumeAll();
}

/**
  * @brief  get the free memory size in sram.
  * @retval size of free momery in sram.
  */
int sram_get_free_size(void)
{
	int free_mem = 0;
	struct sram_Heap *h = &g_sram_heap;
	sramMemChunk *chunk;

	vTaskSuspendAll();
	if (!g_sram_heap_inited)	{
		sram_heap_init();
	}

	for (chunk = h->FreeList; chunk; chunk = chunk->next) {
		free_mem += chunk->size;
	}

	xTaskResumeAll();
	return free_mem;
}

/**
  * @brief  allocate free memory from sram.
  * @param  size: size of the memory you want.
  * @retval: header address of the memory allocated.
  */
void *sram_malloc(int size)
{
	int *mem;

	size += (unsigned int) HEAP_STRUCT_SIZE;
	if ((mem = (int *)sram_heap_allocmem(size))) {
		*mem = size;
		mem = (int *)(((char *)mem) + (unsigned int)HEAP_STRUCT_SIZE);
	}

	return (void *)mem;
}

/**
  * @brief allocate several continuous blocks of free memory from sram.
  * @param  num: num of blocks you want.
  * @param  size: size of every block.
  * @retval: header address of the memory allocated.
  */
void *sram_calloc(int num, int size)
{
	void *mem;

	if ((mem = sram_malloc(num * size))) {
		memset(mem, 0, (unsigned int)(num * size));
	}

	return mem;
}

/**
  * @brief  free memory in sram.
  * @param  mem: header address of the memory to be freed.
  * @retval none
  */
void sram_free(void *mem)
{
	if (mem) {
		int *_mem = (int *)((char *)mem - HEAP_STRUCT_SIZE);
		sram_reserved_heap_freemem(_mem, *_mem);
	}
}


