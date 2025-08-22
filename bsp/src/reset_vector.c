/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

extern int _memmap_reset_vector;

typedef struct _ENTRY_INFO_ {
	u32 entry_addr;
} ENTRY_INFO, *PENTRY_INFO;


__attribute__ ((section(".entry_table"))) const ENTRY_INFO  entry_table[] = {
	{
			(u32)&_memmap_reset_vector
	},
};
