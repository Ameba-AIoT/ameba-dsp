/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_HP_RCC_H_
#define _AMEBA_HP_RCC_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup RCC
  * @brief RCC driver modules
  * @{
  */


/** @defgroup HS_RCC
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  *		- functions prototypes for peripheral reset and clock control driver.
  *
  *****************************************************************************************
  *
  *****************************************************************************************
  * how to use
  *****************************************************************************************
  * use UART0 as example:
  *		RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
  *
  *****************************************************************************************
  * @endverbatim
  */

/** @addtogroup HS_RCC
  * @brief HS_RCC driver modules
  * @{
  */

/* Exported constants --------------------------------------------------------*/

#define APBPeriph_NULL					0x0
#define APBPeriph_CLOCK_NULL			0x0



/** @defgroup VADMEM_Clock_Source_define
  * @{
  */
#define CKSL_VADM_HS_PLFM				0x00
#define CKSL_VADM_VAD					0x01
/**
  * @}
  */

/** @defgroup AUDIOCODEC_Clock_Source_define
  * @{
  */
#define CKSL_AC_SYSPLL					0x00
#define CKSL_AC_XTAL					0x01
/**
  * @}
  */


/** @defgroup I2S_Clock_Source_define
  * @{
  */

#define CKSL_I2S_XTAL40M			0x00
#define CKSL_I2S_CPUPLL				0x01
#define CKSL_I2S_DSPPLL				0x02
/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup HS_RCC_Exported_Functions HS_RCC Exported Functions
  * @{
  */
void RCC_PeriphClockCmd(u32 APBPeriph, u32 APBPeriph_Clock, u8 NewState);
void RCC_PeriphClockSource_SPORT(u32 Source);
void RCC_PeriphClockSource_VADMEM(u32 Source);
void RCC_PeriphClockSource_AUDIOCODEC(u32 Source);

/**
  * @}
  */


#endif /* _AMEBA_HP_RCC_H_ */
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

