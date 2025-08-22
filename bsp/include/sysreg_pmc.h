/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBAD2_PMC_H_
#define _AMEBAD2_PMC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PMC
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup PMC_Register_Definitions PMC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup WAK_STATUS0
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_STA_31_0             ((u32)0xFFFFFFFF << 0)          /*!<R 0  wak evt status. It can't be cleare here, clear at IP side . Bit0: WIFI_FISR_FESR_IRQ Bit1: WIFI_FTSR_MAILBOX_IRQ Bit2: AP_WAKE_IRQ Bit3: DSP_WAKE_IRQ Bit4: BT_WAKE_HOST Bit5: IPC_KR4 Bit6: IPC_KM4 Bit7: IPC_DSP Bit8: IWDG Bit9: Timer0 Bit10: Timer1 Bit11: Timer2 Bit12: Timer3 Bit13: Timer4 Bit14: Timer5 Bit15: Timer6 Bit16: Timer7 Bit17: UART0 Bit18: UART1 Bit19: UART2 Bit20: UART3 Bit21: UART_LOG Bit22: GPIOA Bit23: GPIOB Bit24: CTOUCH Bit25: SPI0 Bit26: SPI1 Bit27: AON_RTS Bit28: ADC Bit29: ADC_COMP Bit30: BOR Bit31: PWR_DOWN */
#define PMC_WAK_STA_31_0(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_STA_31_0(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_STATUS1
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_STA_63_32            ((u32)0xFFFFFFFF << 0)          /*!<R 0  wak evt status. It can't be cleare here, clear at IP side . Bit0: AON_TIMER Bit1: AON_GPIO_WAKE Bit2: VAD Bit3~31: Reserved */
#define PMC_WAK_STA_63_32(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_STA_63_32(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */


/** @defgroup WAK_MASK0_DSP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_DSP_IMR_31_0         ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake DSP */
#define PMC_WAK_DSP_IMR_31_0(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_DSP_IMR_31_0(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup WAK_MASK1_DSP
 * @brief
 * @{
 **/
#define PMC_MASK_WAK_DSP_IMR_63_32        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  1:mask enable for wake DSP */
#define PMC_WAK_DSP_IMR_63_32(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define PMC_GET_WAK_DSP_IMR_63_32(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */


/*==========PMC Register Address Definition==========*/
#define WAK_STATUS0                                  0x0000
#define WAK_STATUS1                                  0x0004
#define WAK_MASK0_DSP                                0x0018
#define WAK_MASK1_DSP                                0x001C

/** @} */

/* AUTO_GEN_END */

/* AUTO_GEN_WAKE_SRC_START */

/**************************************************************************//**
* @defgroup WAK_STATUS0/WAK_MASK0_NP/WAK_MASK0_AP/WAK_MASK0_DSP
* @brief
* @{
*****************************************************************************/
#define WAKE_SRC_WIFI_FISR_FESR                      ((u32)0x00000001 << 0)
#define WAKE_SRC_WIFI_FTSR_MAILBOX                   ((u32)0x00000001 << 1)
#define WAKE_SRC_AP_WAKE                             ((u32)0x00000001 << 2)
#define WAKE_SRC_DSP_WAKE                            ((u32)0x00000001 << 3)
#define WAKE_SRC_BT_WAKE_HOST                        ((u32)0x00000001 << 4)
#define WAKE_SRC_IPC_KR4                             ((u32)0x00000001 << 5)
#define WAKE_SRC_IPC_KM4                             ((u32)0x00000001 << 6)
#define WAKE_SRC_IPC_DSP                             ((u32)0x00000001 << 7)
#define WAKE_SRC_WDG0                                ((u32)0x00000001 << 8)
#define WAKE_SRC_Timer0                              ((u32)0x00000001 << 9)
#define WAKE_SRC_Timer1                              ((u32)0x00000001 << 10)
#define WAKE_SRC_Timer2                              ((u32)0x00000001 << 11)
#define WAKE_SRC_Timer3                              ((u32)0x00000001 << 12)
#define WAKE_SRC_Timer4                              ((u32)0x00000001 << 13)
#define WAKE_SRC_Timer5                              ((u32)0x00000001 << 14)
#define WAKE_SRC_Timer6                              ((u32)0x00000001 << 15)
#define WAKE_SRC_Timer7                              ((u32)0x00000001 << 16)
#define WAKE_SRC_UART0                               ((u32)0x00000001 << 17)
#define WAKE_SRC_UART1                               ((u32)0x00000001 << 18)
#define WAKE_SRC_UART2                               ((u32)0x00000001 << 19)
#define WAKE_SRC_UART3                               ((u32)0x00000001 << 20)
#define WAKE_SRC_UART_LOG                            ((u32)0x00000001 << 21)
#define WAKE_SRC_GPIOA                               ((u32)0x00000001 << 22)
#define WAKE_SRC_GPIOB                               ((u32)0x00000001 << 23)
#define WAKE_SRC_CTOUCH                              ((u32)0x00000001 << 24)
#define WAKE_SRC_SPI0                                ((u32)0x00000001 << 25)
#define WAKE_SRC_SPI1                                ((u32)0x00000001 << 26)
#define WAKE_SRC_AON_RTC                             ((u32)0x00000001 << 27)
#define WAKE_SRC_ADC                                 ((u32)0x00000001 << 28)
#define WAKE_SRC_ADC_COMP                            ((u32)0x00000001 << 29)
#define WAKE_SRC_BOR                                 ((u32)0x00000001 << 30)
#define WAKE_SRC_PWR_DOWN                            ((u32)0x00000001 << 31)
/** @} */

/**************************************************************************//**
* @defgroup WAK_STATUS1/WAK_MASK1_NP/WAK_MASK1_AP/WAK_MASK1_DSP
* @brief
* @{
*****************************************************************************/
#define WAKE_SRC_AON_TIM                             ((u32)0x00000001 << 0)
#define WAKE_SRC_AON_WAKEPIN                         ((u32)0x00000001 << 1)
#define WAKE_SRC_VAD                                 ((u32)0x00000001 << 2)

/** @} */

/* AUTO_GEN_WAKE_SRC_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif

/** @} */

/** @} */
