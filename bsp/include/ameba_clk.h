/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_CLK_H_
#define _AMEBA_CLK_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup CLOCK
  * @brief CLOCK driver modules
  * @{
  */

/** @addtogroup CLOCK
  * @verbatim
  *****************************************************************************************
  * SDM32K
  *****************************************************************************************
  *	-RTC clock in
  *	-TIM0-TIM8 clock in
  *	-WIFI 32K clock in

  *****************************************************************************************
  * KM4 CPU clock
  *****************************************************************************************
  *	-CLK_KM4_200M: 200MHz
  *	-CLK_KM4_100M: 100MHz
  *	-CLK_KM4_50M: 50MHz
  *	-CLK_KM4_25M: 25MHz
  *	-CLK_KM4_XTAL: XTAL
  *
  *****************************************************************************************
  * XTAL clock
  *****************************************************************************************
  *	-Get XTAL clock from EFUSE setting:
  *	-40000000
  *	-25000000
  *	-13000000
  *	-19200000
  *	-20000000
  *	-26000000
  *	-38400000
  *	-17664000
  *	-16000000
  *	-14318000
  *	-12000000
  *	-52000000
  *	-48000000
  *	-26000000
  *	-27000000
  *	-24000000

  *****************************************************************************************
  * EXT32K
  *****************************************************************************************
  *	-External 32K: 32K clock from external 32k source
  *	-Internal 32K: 32K clock from internal 32K source: NCO32K
  *
  *****************************************************************************************
  * @endverbatim
  */

/* Exported constants --------------------------------------------------------*/


/** @defgroup CPU_CLK_Source definitions
  * @{
  */

#define CLK_CPU_DPLL		3
#define CLK_CPU_MPLL		2
/**
  * @}
  */



/** @defgroup XTAL_CLK_Exported_Functions XTAL_CLK Exported Functions
  * @{
  */
u32 XTAL_ClkGet(void);
u32 PLL_ClkGet(u8 pll_type);
/**
  * @}
  */



/* Registers Definitions --------------------------------------------------------*/

/**************************************************************************//**
 * @defgroup REG_SYS_SYSPLL_CTRL2
 * @{
 *****************************************************************************/
#define BIT_SYS_SYSPLL_CK_ADC_EN		(0x00000001 << 25) /*!< Set ADC PLL EN */
/** @} */

/** @defgroup PLL_BandGap_Mask_definitions
  * @{
  */
#define PLL_BG_POW_MASK	((u32)0x00000005)

/** @} */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#endif //_AMEBA_CLK_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
