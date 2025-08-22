/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"


/**
  * @brief  Fills each WDG_InitStruct member with its default value.
  * @param  WDG_InitStruct: pointer to an WDG_InitTypeDef structure which will be initialized.
  * @retval None
  */
__weak void WDG_Wait_Busy(WDG_TypeDef *WDG)
{
	u32 times = 0;

	/* Wait for no update event */
	while (WDG->WDG_CR & (WDG_BIT_RVU | WDG_BIT_EVU)) {
		times++;
		DelayUs(1);

		if (times > 1000) {
			
			break;
		}
	}
}

/**
  * @brief  Fills each WDG_InitStruct member with its default value.
  * @param  WDG_InitStruct: pointer to an WDG_InitTypeDef structure which will be initialized.
  * @retval None
  */
__weak void WDG_StructInit(WDG_InitTypeDef *WDG_InitStruct)
{
	/* Load HAL initial data structure default value */
	WDG_InitStruct->Window     = 0xFFFF;
	WDG_InitStruct->Timeout     = 0xFFFF;
	WDG_InitStruct->EICNT     = 0x00;
	WDG_InitStruct->EIMOD     = DISABLE;
}

/**
  * @brief  Initializes the  WDG registers according to the specified parameters
  * @param  WDG where WDG can be IWDG_DEV or WDG1~4
  * @param  WDG_InitStruct: pointer to a WDG_InitTypeDef structure that contains
  *         the configuration information for the WDG peripheral.
  * @retval None
  */
__weak void WDG_Init(WDG_TypeDef *WDG, WDG_InitTypeDef *WDG_InitStruct)
{
	u32 prescaler = 0;

	assert_param(IS_WDG_ALL_PERIPH(WDG));

	if (IS_IWDG_PERIPH(WDG)) {
		prescaler = 0x63;
	} else {
		prescaler = 0x1F;
	}

	WDG_Wait_Busy(WDG);

	/*Enable Register access*/
	WDG->WDG_MKEYR = WDG_ACCESS_EN;


	if (WDG_InitStruct->EIMOD) {
		WDG->WDG_CR = WDG_BIT_EIE | WDG_EICNT(WDG_InitStruct->EICNT);
	} else {
		WDG->WDG_CR = 0;
	}

	WDG->WDG_RLR = WDG_PRER(prescaler) | WDG_RELOAD(WDG_InitStruct->Timeout);
	WDG->WDG_WINR = WDG_InitStruct->Window;

	/*Disable Register access*/
	WDG->WDG_MKEYR = 0xFFFF;

}

/**
  * @brief  Enable WDG
  * @param  WDG where WDG can be IWDG_DEV or WDG1~4
  * @note   Once enabled ,the WDG can't Disabled by software
  * @retval None
  */
__weak void WDG_Enable(WDG_TypeDef *WDG)
{
	assert_param(IS_WDG_ALL_PERIPH(WDG));

	WDG_Wait_Busy(WDG);
	WDG->WDG_MKEYR = WDG_FUNC_EN;
}

/**
  * @brief  Enable WDG
  * @param  WDG where WDG can be IWDG_DEV or WDG1~4
  * @param  Timeout specify the target timeout period in ms
  * @note   WDG won't use the new timeout until a WDG refresh,Be careful
  *		 if window pretection enabled.
  * @retval None
  */
__weak void WDG_Timeout(WDG_TypeDef *WDG, u32 Timeout)
{
	u32 prescaler = 0;

	assert_param(IS_WDG_ALL_PERIPH(WDG));

	if (IS_IWDG_PERIPH(WDG)) {
		prescaler = 0x63;
	} else {
		prescaler = 0x1F;
	}

	WDG_Wait_Busy(WDG);

	/*Enable Register access*/
	WDG->WDG_MKEYR = WDG_ACCESS_EN;

	WDG->WDG_RLR = WDG_PRER(prescaler) | WDG_RELOAD(Timeout);

	/*Disable Register access*/
	WDG->WDG_MKEYR = 0xFFFF;

}


/**
  * @brief  Refresh WDG timer
  * @param  WDG where WDG can be IWDG_DEV or WDG1~4
  * @note   If call this function to refresh WDG before timeout period,
  *			then MCU reset or WDG interrupt won't generate
  * @retval None
  */
__weak void WDG_Refresh(WDG_TypeDef *WDG)
{
	assert_param(IS_WDG_ALL_PERIPH(WDG));

	WDG_Wait_Busy(WDG);

	WDG->WDG_MKEYR = WDG_REFRESH;
}

/**
  * @brief  Clear WDG interrupt when WDG init use WDG_IrqInit .
  * @param  WDG where WDG can be IWDG_DEV or WDG1~4
  * @param  WDG_IT specifies the interrupt source to be enabled or disabled
  * @param  NewState specifies the state of the interrupt.
  * @note   This function only used in interrupt mode
  * @retval None
  */
__weak void WDG_INTConfig(WDG_TypeDef *WDG, u32 WDG_IT, u32 NewState)
{
	assert_param(IS_WDG_ALL_PERIPH(WDG));
	assert_param(WDG_IT == WDG_BIT_EIE);

	WDG_Wait_Busy(WDG);

	/*Enable Register access*/
	WDG->WDG_MKEYR = WDG_ACCESS_EN;

	if (NewState == ENABLE) {
		WDG->WDG_CR |= WDG_IT;
	} else {
		WDG->WDG_CR &= ~WDG_IT;
	}

	/*Disable Register access*/
	WDG->WDG_MKEYR = 0xFFFF;
}

/**
  * @brief  Clear WDG interrupt .
  * @param  where WDG can be IWDG_DEV or WDG1~4
  * @param  INTrBit specifies the interrupt sources to be cleared
  * @retval None
  */
__weak void WDG_ClearINT(WDG_TypeDef *WDG, u32 INTrBit)
{
	assert_param(IS_WDG_ALL_PERIPH(WDG));
	assert_param(INTrBit == WDG_BIT_EIC);

	WDG_Wait_Busy(WDG);

	/*Enable Register access*/
	WDG->WDG_MKEYR = WDG_ACCESS_EN;

	/* Clear ISR */
	WDG->WDG_CR |= INTrBit;

	/*Disable Register access*/
	WDG->WDG_MKEYR = 0xFFFF;
}

/**
  * @brief  Enable or Disable IWDG low power function.
  * @param  where WDG can only be IWDG_DEV
  * @param  NewState: specifies the state of the low power function.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
__weak void IWDG_LP_Enable(WDG_TypeDef *WDG, u32 NewState)
{
	assert_param(IS_IWDG_PERIPH(WDG));

	WDG_Wait_Busy(WDG);

	/*Enable Register access*/
	WDG->WDG_MKEYR = WDG_ACCESS_EN;

	if (NewState == ENABLE) {
		WDG->WDG_CR |= WDG_BIT_LPEN;
	} else {
		WDG->WDG_CR &= ~WDG_BIT_LPEN;
	}

	/*Disable Register access*/
	WDG->WDG_MKEYR = 0xFFFF;
}

/**
  * @brief  Enable or Disable WDG bypass function.
  * @param  where WDG can be WDG1~4
  * @param  NewState: specifies the state of the bypass function.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  * @note   WDG can only trigger global reset if bypass is enabled
  */
__weak void WDG_Reset_Bypass(WDG_TypeDef *WDG, u32 NewState)
{
	assert_param(IS_SYETEM_WDG_PERIPH(WDG));

	u32 controlbit = 0;
	u32 temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_WDG_GLBRST);

	if (WDG == WDG1_DEV) {
		controlbit = LSYS_BIT_EN_KM4_RST_GLB;
	} else if (WDG == WDG2_DEV) {
		controlbit = LSYS_BIT_EN_KM4_RST_GLB;
	} else if (WDG == WDG3_DEV) {
		controlbit = LSYS_BIT_EN_KR4_RST_GLB;
	} else if (WDG == WDG4_DEV) {
		controlbit = LSYS_BIT_EN_HIFI_RST_GLB;
	}

	if (NewState == ENABLE) {
		temp |= controlbit;
	} else {
		temp &= ~(controlbit);
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_WDG_GLBRST, temp);
}

/**
  * @brief  Set Reset Timeout Value
  * @param  Timeout specify the target timeout period, unit is 10us, 0 means not support
  * @retval None
  */
__weak void WDG_Reset_Timeout(u32 Timeout)
{
	u32 temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_WDG_GLBRST);
	temp &= ~(LSYS_MASK_WDG_TIMEOUT);
	temp |= LSYS_WDG_TIMEOUT(Timeout);

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_WDG_GLBRST, temp);
}

/**
  * @brief  Clear WDG interrupt when WDG cpu reset happens.
  * @param  WDG where WDG can be WDG1~4
  * @note   This function only used in watchdog cpu reset, not for watchdog global reset
  * @retval None
  */
__weak void WDG_Reset_ClearINT(WDG_TypeDef *WDG)
{
	assert_param(IS_SYETEM_WDG_PERIPH(WDG));

	u32 temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_WDG_GLBRST);

	if (WDG == WDG1_DEV) {
		temp |= LSYS_BIT_CLR_WDG1_RST;
	} else if (WDG == WDG2_DEV) {
		temp |= LSYS_BIT_CLR_WDG2_RST;
	} else if (WDG == WDG3_DEV) {
		temp |= LSYS_BIT_CLR_WDG3_RST;
	} else if (WDG == WDG4_DEV) {
		temp |= LSYS_BIT_CLR_WDG4_RST;
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_WDG_GLBRST, temp);
}

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
