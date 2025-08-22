/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

/**
  * @brief  Enables or disables the APB peripheral clock and function
  * @param  APBPeriph: specifies the APB peripheral to gates its clock.
  *      this parameter can be one of @ref UART_IRDA_PULSE_SHIFT_define
  * @param  APBPeriph_Clock: specifies the APB peripheral clock config.
  *      this parameter can be one of @ref RCC_FUNC_Exported_Constants
  * @param  NewState: new state of the specified peripheral clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
__weak void RCC_PeriphClockCmd(u32 APBPeriph_in, u32 APBPeriph_Clock_in, u8 NewState)
{
	u32 ClkRegIndx = (APBPeriph_Clock_in >> 30) & 0x03;
	u32 APBPeriph_Clock = APBPeriph_Clock_in & (~(BIT(31) | BIT(30)));

	u32 FuncRegIndx = (APBPeriph_in >> 30) & 0x03;
	u32 APBPeriph = APBPeriph_in & (~(BIT(31) | BIT(30)));

	u32 Reg = 0;
	u32 TempVal = 0;

	//DBG_8195A("RCC_PeriphClockCmd %x %x %x\n", APBPeriph_in, APBPeriph_Clock_in, NewState);

	//clock
	switch (ClkRegIndx) {
	case 0x0:
		Reg = REG_LSYS_CKE_GRP0;
		break;
	case 0x1:
		Reg = REG_LSYS_CKE_GRP1;
		break;
	case 0x3:
		Reg = REG_AON_CLK;
		break;
	}

	if (APBPeriph_Clock_in != APBPeriph_CLOCK_NULL) {
		if (NewState == ENABLE) {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE, Reg);
			TempVal |= APBPeriph_Clock;
			HAL_WRITE32(SYSTEM_CTRL_BASE, Reg, TempVal);
		} else {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE, Reg);
			TempVal &= ~APBPeriph_Clock;
			HAL_WRITE32(SYSTEM_CTRL_BASE, Reg, TempVal);
		}
	}

	//function
	switch (FuncRegIndx) {
	case 0x0:
		Reg = REG_LSYS_FEN_GRP0;
		break;
	case 0x1:
		Reg = REG_LSYS_FEN_GRP1;
		break;
	case 0x3:
		Reg = REG_AON_FEN;
		break;
	}

	if (APBPeriph_in != APBPeriph_NULL) {
		if (NewState == ENABLE) {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE, Reg);
			TempVal |= APBPeriph;
			HAL_WRITE32(SYSTEM_CTRL_BASE, Reg, TempVal);
		} else {
			TempVal = HAL_READ32(SYSTEM_CTRL_BASE, Reg);
			TempVal &= ~APBPeriph;
			HAL_WRITE32(SYSTEM_CTRL_BASE, Reg, TempVal);
		}
	}

	DelayUs(10);

	return;
}


/**
  * @brief    Configure vadmem Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_VADM_HS_PLFM
  *                            @arg CKSL_VADM_VAD
  * @retval   None
  * @note    Used to switch VADM clock
  */
__weak void RCC_PeriphClockSource_VADMEM(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_VADM);
	Temp |= LSYS_CKSL_VADM(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Temp);
}

/**
  * @brief    Configure audiocodec Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_AC_XTAL
  *                            @arg CKSL_AC_SYSPLL
  * @retval   None
  * @note    Used to switch AC clock
  */
__weak void RCC_PeriphClockSource_AUDIOCODEC(u32 Source)
{
	u32 Temp = 0;

	Temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);
	Temp &= ~(LSYS_BIT_CKSL_AC);
	Temp |= LSYS_CKSL_AC(Source);
	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, Temp);
}



/**
  * @brief    Configure sport Clock.
  * @param  Source:  This parameter can be one of the following values:
  *                            @arg CKSL_I2S_XTAL40M
  *                            @arg CKSL_I2S_CPUPLL
  * @retval   None
  * @note    Used to switch SPORT clock
  */
__weak void RCC_PeriphClockSource_SPORT(u32 Source)
{
	u32 RegValue = 0;

	RegValue = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0);

	if (Source == CKSL_I2S_XTAL40M) {
		RegValue &= ~(LSYS_BIT_CKSL_SPORT);
	} else {
		RegValue |= (LSYS_BIT_CKSL_SPORT);

		if (Source == CKSL_I2S_CPUPLL) {
			RegValue |= (LSYS_BIT_CKSL_I2S_PLL);
		} else {
			RegValue &= ~(LSYS_BIT_CKSL_I2S_PLL);
		}
	}

	HAL_WRITE32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP0, RegValue);
}



/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
