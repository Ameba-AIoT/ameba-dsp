/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_VECTOR_TABLE_H_
#define _AMEBA_VECTOR_TABLE_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup IRQ
  * @brief IRQ modules
  * @{
  */

/** @addtogroup IRQ
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * IRQ table, please refer to IRQ Exported Constants->IRQn_enum->IRQn
  *
  *****************************************************************************************
  * how to use
  *****************************************************************************************
  *		1. register/unregister IRQ use: InterruptRegister/InterruptUnRegister
  *		2. enable/disable IRQ use: InterruptEn/InterruptDis
  *
  *****************************************************************************************
  * @endverbatim
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup DSPIRQn_enum
  * @{
  */
enum DSPRQn {

	/******  Specific Interrupt Numbers ************/
	WL_DMA_IRQ_DSP = 9,
	IPC_DSP_IRQ_DSP = 7,
	DSP_WDG_INT_IRQ_DSP = 8,
	TIMER7_IRQ_DSP = 10,
	TIMER10_IRQ_DSP = 11,
	TIMER11_IRQ_DSP = 12,
	TIMER12_IRQ_DSP = 13,
	TIMER13_IRQ_DSP = 14,
	TIMER14_IRQ_DSP = 15,
	GPIOA_IRQ_DSP = 16,
	GPIOB_IRQ_DSP = 17,
	CTOUCH_IRQ_DSP = 18,
	GDMA0_CHANNEL0_IRQ_DSP = 19,
	GDMA0_CHANNEL1_IRQ_DSP = 20,
	GDMA0_CHANNEL2_IRQ_DSP = 21,
	GDMA0_CHANNEL3_IRQ_DSP = 22,
	SPORT0_IRQ_DSP = 23,
	SPORT1_IRQ_DSP = 24,
	VAD_IRQ_DSP = 25,
	ZIGBEE_IRQ_DSP = 26,
	DSP_NMI_IRQ_DSP = 28,
};
/**
  * @}
  */

/**
  * @}
  */

#endif //_AMEBA_VECTOR_TABLE_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/

