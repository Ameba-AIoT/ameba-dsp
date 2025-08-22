/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SRAM_HEAP_H_
#define _SRAM_HEAP_H_

void *sram_malloc(int size);
void *sram_calloc(int num, int size);
void sram_free(void *mem);
int sram_get_free_size(void);


#endif /* _SRAM_HEAP_H_ */
