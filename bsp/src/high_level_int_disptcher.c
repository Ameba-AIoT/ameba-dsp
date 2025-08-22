/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <xtensa/config/core.h>
typedef struct xt_handler_table_entry {
	void *handler;
	void *arg;
} xt_handler_table_entry;

extern xt_handler_table_entry _xt_interrupt_table[XCHAL_NUM_INTERRUPTS];

#define   LEVEL_4_INTNUM_0   26
#define   LEVEL_4_INTNUM_1   20
#define   LEVEL_4_INTNUM_2   19
#define   LEVEL_4_INTNUM_3   8

#define   LEVEL_3_INTNUM_0   25
#define   LEVEL_3_INTNUM_1   24
#define   LEVEL_3_INTNUM_2   23
#define   LEVEL_3_INTNUM_3   15
#define   LEVEL_3_INTNUM_4   14
#define   LEVEL_3_INTNUM_5   0

void dispatch_level_4()
{
	uint32_t a = xthal_get_interrupt();
	uint32_t b = xthal_get_intenable();
	uint32_t c = XCHAL_INTLEVEL4_MASK;

	uint32_t temp = ((a & b) & c);
	xt_handler_table_entry *entry;

	void (*f_ptr)(int *p_fun);

	if (temp & (1 << LEVEL_4_INTNUM_0)) {
		entry = _xt_interrupt_table + LEVEL_4_INTNUM_0;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_4_INTNUM_1)) {
		entry = _xt_interrupt_table + LEVEL_4_INTNUM_1;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_4_INTNUM_2)) {
		entry = _xt_interrupt_table + LEVEL_4_INTNUM_2;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_4_INTNUM_3)) {
		entry = _xt_interrupt_table + LEVEL_4_INTNUM_3;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}
}


void dispatch_level_3()
{
	uint32_t a = xthal_get_interrupt();
	uint32_t b = xthal_get_intenable();
	uint32_t c = XCHAL_INTLEVEL3_MASK;

	uint32_t temp = ((a & b) & c);
	xt_handler_table_entry *entry;

	void (*f_ptr)(int *p_fun);

	if (temp & (1 << LEVEL_3_INTNUM_0)) {
		entry = _xt_interrupt_table + LEVEL_3_INTNUM_0;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_3_INTNUM_1)) {
		entry = _xt_interrupt_table + LEVEL_3_INTNUM_1;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_3_INTNUM_2)) {
		entry = _xt_interrupt_table + LEVEL_3_INTNUM_2;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_3_INTNUM_3)) {
		entry = _xt_interrupt_table + LEVEL_3_INTNUM_3;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_3_INTNUM_4)) {
		entry = _xt_interrupt_table + LEVEL_3_INTNUM_4;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

	if (temp & (1 << LEVEL_3_INTNUM_5)) {
		entry = _xt_interrupt_table + LEVEL_3_INTNUM_5;
		f_ptr = entry->handler;
		f_ptr(entry->arg);
	}

}
