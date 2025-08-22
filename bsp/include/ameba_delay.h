/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_DELAY_H_
#define _AMEBA_DELAY_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup DELAY
  * @brief DELAY driver modules
  * @{
  */

/** @addtogroup DELAY
  * @verbatim
  *****************************************************************************************
  * Delay
  *****************************************************************************************
  * 	- DelayUs
  * 	- DelayMs
  * 	- DelayNop
  *
  *****************************************************************************************
  * Sys Timer
  *****************************************************************************************
  * 	- TIM0 is used as systimer, so TIM0 can not be used for other purpose
  * 	- init when boot in rom code
  *	- resolution is 31us
  *	- you can get timer tick use SYSTIMER_TickGet, every tick is 31us
  *	- you can get passing time use SYSTIMER_GetPassTime in ms
  *
  *****************************************************************************************
  * @endverbatim
  */

/** @defgroup DELAY_Exported_Functions DELAY Exported Functions
  * @{
  */
void DelayUs(u32 us);
void DelayMs(u32 ms);
void DelayNop(u32 count);
u32 SYSTIMER_TickGet(void);
u32 SYSTIMER_GetPassTick(u32 start);
u32 SYSTIMER_GetPassTime(u32 start);
u32 CPU_InInterrupt(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#endif//_AMEBA_DELAY_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
