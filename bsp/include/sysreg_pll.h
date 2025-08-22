/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBAD2_PLL_H_
#define _AMEBAD2_PLL_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PLL
* @{
*/

/* Registers Definitions --------------------------------------------------------*/


/** @defgroup PLL_CPUPLL_CTRL1
 * @brief
 * @{
 **/
#define PLL_GET_CPUPLL_DIVN_SDM(x)            ((u32)(((x >> 5) & 0x0000003F)))
/** @} */


/** @defgroup PLL_CPUPLL_CTRL3
 * @brief
 * @{
 **/

#define PLL_GET_CPUPLL_F0F_SDM(x)             ((u32)(((x >> 19) & 0x00001FFF)))
#define PLL_GET_CPUPLL_F0N_SDM(x)             ((u32)(((x >> 16) & 0x00000007)))
/** @} */


/* Exported types --------------------------------------------------------*/

/** @defgroup PLL_Exported_Types PLL Exported Types
  * @{
*****************************************************************************/


/**
 * @brief PLL Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t PLL_CPUPLL_CTRL0;                       /*!<  Register,  Address offset: 0x000 */
	__IO uint32_t PLL_CPUPLL_CTRL1;                       /*!<  Register,  Address offset: 0x004 */
	__IO uint32_t PLL_CPUPLL_CTRL2;                       /*!<  Register,  Address offset: 0x008 */
	__IO uint32_t PLL_CPUPLL_CTRL3;                       /*!<  Register,  Address offset: 0x00C */
	__IO uint32_t PLL_PERIPLL_CTRL0;                      /*!<  Register,  Address offset: 0x010 */
	__IO uint32_t PLL_PERIPLL_CTRL1;                      /*!<  Register,  Address offset: 0x014 */
	__IO uint32_t PLL_PERIPLL_CTRL2;                      /*!<  Register,  Address offset: 0x018 */
	__IO uint32_t PLL_PERIPLL_CTRL3;                      /*!<  Register,  Address offset: 0x01C */
	__IO uint32_t PLL_TEST;                               /*!<  Register,  Address offset: 0x020 */
	__IO uint32_t PLL_CPUPLL_PS;                          /*!<  Register,  Address offset: 0x024 */
	__IO uint32_t PLL_PERIPLL_PS;                         /*!<  Register,  Address offset: 0x028 */
	__IO uint32_t PLL_AUX_BG;                             /*!<  Register,  Address offset: 0x02C */
	__I  uint32_t PLL_STATE;                              /*!<  Register,  Address offset: 0x030 */
	__IO uint32_t PLL_DUMMY;                              /*!<  Register,  Address offset: 0x034 */
} PLL_TypeDef;

#define PLL_BASE ((PLL_TypeDef *) (0x4100C440))

/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
