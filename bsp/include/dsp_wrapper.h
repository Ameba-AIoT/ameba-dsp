/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BASIC_TYPES_H__
#define __BASIC_TYPES_H__


#include <stdint.h>
#include <stddef.h> /* for size_t */
#include <stdbool.h>

#define ALIGN(x) __attribute__((aligned(x)))

#define DRAM0_DATA __attribute__((section(".dram0.data")))
#define DRAM1_DATA __attribute__((section(".dram1.data")))
#define SRAM_DATA __attribute__((section(".sram_dsp.data")))

#define DRAM0_BSS __attribute__((section(".dram0.bss")))
#define DRAM1_BSS __attribute__((section(".dram1.bss")))
#define SRAM_BSS __attribute__((section(".sram_dsp.bss")))

typedef   int8_t   s8;
typedef   uint8_t   u8;
typedef   int16_t   s16;
typedef   uint16_t   u16;
typedef   int32_t   s32;
typedef   uint32_t   u32;
typedef   uint64_t   u64;
typedef   s32    IRQn_Type;

#define BOOL int

#define   ON        1
#define   OFF       0

#define   ENABLE    1
#define   DISABLE   0


#define BIT(__n)       (1U<<(__n))

#define __STATIC_INLINE           static __attribute__((always_inline))
#define __weak                  __attribute__((weak))

#define   DCache_Clean   xthal_dcache_region_writeback
#define   DCache_Invalidate   xthal_dcache_region_invalidate
#define   DCache_CleanInvalidate   xthal_dcache_region_writeback_inv

#define   IRQ_FUN   xt_handler

#define   InterruptRegister(a, b, c, d)   xt_set_interrupt_handler(b, a, (void*)(c))
#define   InterruptUnRegister(a)   xt_set_interrupt_handler(a, NULL, NULL)
#define   InterruptEn(a,b)   xt_interrupt_enable(a)
#define   InterruptDis(a)   xt_interrupt_disable(a)

#endif
