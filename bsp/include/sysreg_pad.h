/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBAD2_PAD_H_
#define _AMEBAD2_PAD_H_


/**************************************************************************//**
 * @defgroup REG_GPIOx
 * @brief GPIO group control
 * @{
 *****************************************************************************/
#define PAD_BIT_GPIOx_DIS           ((u32)0x00000001 << 15)          /*!<R/W 0h  PAD shutdown, only when the whole group's dis signals are all 1, the PAD SHDN will pull down to shutdown the group pad. 1. disable pad 0. enable pad */
#define PAD_BIT_GPIOx_SR            ((u32)0x00000001 << 13)          /*!<R/W 1h  PAD srew rate control */
#define PAD_BIT_GPIOx_SMT           ((u32)0x00000001 << 12)          /*!<R/W 1h  PAD Schmit control */
#define PAD_BIT_GPIOx_E2            ((u32)0x00000001 << 11)          /*!<R/W 1h  PAD driving ability control. 0: low 1: high The actual driving current is depend on pad type. */
#define PAD_BIT_GPIOx_PUPDC         ((u32)0x00000001 << 10)          /*!<R/W 0h  Some pad may have two type of PU/PD resistor, this bit can select it. 1: small resistor 0: big resistor */
#define PAD_MASK_GPIOx_SEL          ((u32)0x000000FF << 0)          /*!<R/W/ES 0h  PAD pinmux function ID selection */
#define PAD_GPIOx_SEL(x)            ((u32)(((x) & 0x000000FF) << 0))
#define PAD_GET_GPIOx_SEL(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PU_1
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PA_PU              ((u32)0xFFFFFFFF << 0)          /*!<R/W 100013h  PAD pull up enable when system is in active. */
#define PAD_PA_PU(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PU(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PU_2
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PB_PU              ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull up enable when system is in active. */
#define PAD_PB_PU(x)                ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PU(x)            ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PD_1
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PA_PD              ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in active. */
#define PAD_PA_PD(x)                ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PD(x)            ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PD_2
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PB_PD              ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in active. */
#define PAD_PB_PD(x)                ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PD(x)            ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PU_SLP_1
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PA_PU_SLP          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  PAD pull up enable when system is in sleep. */
#define PAD_PA_PU_SLP(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PU_SLP(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PU_SLP_2
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PB_PU_SLP          ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull up enable when system is in sleep. */
#define PAD_PB_PU_SLP(x)            ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PU_SLP(x)        ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PD_SLP_1
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PA_PD_SLP          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in sleep. */
#define PAD_PA_PD_SLP(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PAD_GET_PA_PD_SLP(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PD_SLP_2
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_PB_PD_SLP          ((u32)0x000FFFFF << 0)          /*!<R/W 0h  PAD pull down enable when system is in sleep. */
#define PAD_PB_PD_SLP(x)            ((u32)(((x) & 0x000FFFFF) << 0))
#define PAD_GET_PB_PD_SLP(x)        ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup REG_PAD_AUD_PAD_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PAD_MASK_GPIOE_IE           ((u32)0x000001FF << 0)          /*!<R/W 0h  Audio share pad input enable. */
#define PAD_GPIOE_IE(x)             ((u32)(((x) & 0x000001FF) << 0))
#define PAD_GET_GPIOE_IE(x)         ((u32)(((x >> 0) & 0x000001FF)))
/** @} */


/**************************************************************************//**
 * @defgroup REG_SWD_SDD_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define PAD_BIT_KM4_SWD_SEL         ((u32)0x00000001 << 3)          /*!<R/W 1  select the share swd signal to KM4 connection write the inverse value to take effect */
#define PAD_BIT_SWD_PMUX_EN         ((u32)0x00000001 << 0)          /*!<R/W 1  1: Force enable share SWD pinmux function 0: Disable */
/** @} */


// /*==========PAD Register Address Definition==========*/
#define REG_PU_1                                     0x0100
#define REG_PU_2                                     0x0104
#define REG_PD_1                                     0x0108
#define REG_PD_2                                     0x010C
#define REG_PU_SLP_1                                 0x0110
#define REG_PU_SLP_2                                 0x0114
#define REG_PD_SLP_1                                 0x0118
#define REG_PD_SLP_2                                 0x011C
#define REG_PAD_AUD_PAD_CTRL                         0x01D8
#define REG_SWD_SDD_CTRL                             0x01F4


// /* MANUAL_GEN_START */

// //Please add your defination here

/**************************************************************************//**
 * @defgroup AMEBAD_PINMUX
 * @{
 * @brief AMEBAD_PINMUX Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t REG_GPIOx[52];			/*!< Pad control register */
} PINMUX_TypeDef;
/** @} */




// /* MANUAL_GEN_END */

#endif
