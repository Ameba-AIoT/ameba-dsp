/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

/**
  * @brief  Get TIM0 tick, unit is 31us.
  * @param  none
  */
u32 SYSTIMER_TickGet(void)
{
	return RTIM_GetCount(TIM0);
}


/**
  * @brief  Get TIM0 pass tick.
  * @param  none
  */
u32 SYSTIMER_GetPassTick(u32 start)
{
	u32 current = 0;
	u32 passed = 0;

	current = RTIM_GetCount(TIM0);

	if (current >= start) {
		passed = current - start;
	} else {
		passed = 0xFFFFFFFF - (start - current);
	}

	return passed;
}

/**
  * @brief  Get TIM0 pass time, unit is ms.
  * @param  start: start time stamp (ms)
  * @note resolution is 31us
  */
u32 SYSTIMER_GetPassTime(u32 start)
{
	u32 current = 0;
	u32 passed = 0;

	current = RTIM_GetCount(TIM0);

	if (current >= start) {
		passed = current - start;
	} else {
		passed = 0xFFFFFFFF - (start - current);
	}

	/* ms =x*1000/32768 = (x *1000) >>15 */
	passed = (u32)(((u64)(passed) * 1000) >> 15);

	return passed;
}


u32 CPU_InInterrupt(void)
{
	uint32_t ps_reg;
	asm volatile("rsr %0, ps" : "=a"(ps_reg));
	return (ps_reg & 0x0F) != 0;
}


/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
