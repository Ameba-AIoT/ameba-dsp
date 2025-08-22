/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBAD2_AON_H_
#define _AMEBAD2_AON_H_


/**************************************************************************//**
 * @defgroup REG_AON_FEN
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_LPON                     ((u32)(3  << 30) | (0x00000001 << 16))  /*!<R/W 0  LS global reset 1: Enable LS function 0: Disable */
#define APBPeriph_OTPC                     ((u32)(3  << 30) | (0x00000001 << 7))   /*!<R/W 0  1: Enable OTP function 0: Disable */
#define APBPeriph_SIC                      ((u32)(3  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: enable SIC function 0: reset */
#define APBPeriph_SDM32K                   ((u32)(3  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: Enable 32.768K SDM function 0: Disable */
#define APBPeriph_RTC                      ((u32)(3  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable RTC function 0: Disable */
#define APBPeriph_ATIM                     ((u32)(3  << 30) | (0x00000001 << 2))   /*!<R/W 0  1: Enable AON timer 0: Reset AON timer Note: AON timer starts to run only when event happens. */
#define APBPeriph_IWDG                     ((u32)(3  << 30) | (0x00000001 << 1))   /*!<R/WE 0  1: Enable IWDG by HW autoload. Once enabled, IWDG can't be disabled unless por asserts. */
#define APBPeriph_BOR                      ((u32)(3  << 30) | (0x00000001 << 0))   /*!<R/W 0  Enable bor reset function, default is 0 when power on */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_CLK
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_OTPC_CLOCK               ((u32)(3  << 30) | (0x00000001 << 7))   /*!<R/W 0  1: Enable OTPC clock 0: Disable */
#define APBPeriph_SIC_CLOCK                ((u32)(3  << 30) | (0x00000001 << 5))   /*!<R/W 0  1: Enable SIC clock */
#define APBPeriph_SDM32K_CLOCK             ((u32)(3  << 30) | (0x00000001 << 4))   /*!<R/W 0  1: Enable 32.768K SDM module's clock 0: Disable */
#define APBPeriph_RTC_CLOCK                ((u32)(3  << 30) | (0x00000001 << 3))   /*!<R/W 0  1: Enable RTC clock 0: Disable */
#define APBPeriph_ATIM_CLOCK               ((u32)(3  << 30) | (0x00000001 << 2))   /*!<R/W 1  1: Enable AON timer clock 0: Disable */
#define APBPeriph_AON_CLOCK                ((u32)(3  << 30) | (0x00000001 << 0))   /*!<R/W 1  if 0, AON clock will be gated except PMC and this register. Note: AON clock can't be shutdown by analog need. */
/** @} */

/**************************************************************************//**
 * @defgroup REG_AON_CKSL
 * @brief
 * @{
 *****************************************************************************/
#define AON_MASK_CKSL_32K                  ((u32)0x00000003 << 4)          /*!<R/W 0h  This field is used to select 32K clock. 00/01: SDM32K 10: Crystal 32K 11: External 32K */
#define AON_CKSL_32K(x)                    ((u32)(((x) & 0x00000003) << 4))
#define AON_GET_CKSL_32K(x)                ((u32)(((x >> 4) & 0x00000003)))
#define AON_BIT_CKSL_SDM32K                ((u32)0x00000001 << 3)          /*!<R/W 0h  sdm 128 clock select , this must be fixed after initial selection. 0:osc128 1:xtal lps 312.5K */
#define AON_BIT_CKSL_OTPC                  ((u32)0x00000001 << 1)          /*!<R/W 0h  This field is used to select OTP clk 1: XLAL 0:OSC4M */
#define AON_BIT_OTPC_FLG                   ((u32)0x00000001 << 0)          /*!<R/W 0h  This field is used to indicate OTP autoload field 1: ls 0:aon */
/** @} */


// /*==========AON Register Address Definition==========*/

#define REG_AON_FEN                                  0x0008
#define REG_AON_CLK                                  0x000C
#define REG_AON_CKSL                                 0x0010


// /* MANUAL_GEN_START */

// //Please add your defination here

// /* MANUAL_GEN_END */

#endif


