/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"


/**
  *  @brief Set the Internal pad pull type.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param PullType : the pull type for the pin.This parameter can be one of the following values:
  *		 @arg GPIO_PuPd_NOPULL
  *		 @arg GPIO_PuPd_DOWN
  *		 @arg GPIO_PuPd_UP
  *  @retval None
  */
__weak void PAD_PullCtrl(u8 PinName, u8 PullType)
{
	u32 Regpu = 0;
	u32 Regpd = 0;
	u8 PortNum = PORT_NUM(PinName);
	u8 PinNum = PIN_NUM(PinName);

	if (PortNum == 0) { /* GPIO A */
		Regpu = HAL_READ32(PINMUX_REG_BASE, REG_PU_1);
		Regpd = HAL_READ32(PINMUX_REG_BASE, REG_PD_1);

		if (PullType == GPIO_PuPd_DOWN) {
			Regpd |= BIT(PinNum);
			Regpu &= ~(BIT(PinNum));

			HAL_WRITE32(PINMUX_REG_BASE, REG_PU_1, Regpu);
			HAL_WRITE32(PINMUX_REG_BASE, REG_PD_1, Regpd);
		} else if (PullType == GPIO_PuPd_UP) {
			Regpu |= BIT(PinNum);
			Regpd &= ~(BIT(PinNum));

			HAL_WRITE32(PINMUX_REG_BASE, REG_PD_1, Regpd);
			HAL_WRITE32(PINMUX_REG_BASE, REG_PU_1, Regpu);
		} else if (PullType == GPIO_PuPd_NOPULL) {
			Regpu &= ~(BIT(PinNum));
			Regpd &= ~(BIT(PinNum));

			HAL_WRITE32(PINMUX_REG_BASE, REG_PU_1, Regpu);
			HAL_WRITE32(PINMUX_REG_BASE, REG_PD_1, Regpd);
		}
	} else { /* GPIO B */
		Regpu = HAL_READ32(PINMUX_REG_BASE, REG_PU_2);
		Regpd = HAL_READ32(PINMUX_REG_BASE, REG_PD_2);

		if (PullType == GPIO_PuPd_DOWN) {
			Regpd |= BIT(PinNum);
			Regpu &= ~(BIT(PinNum));

			HAL_WRITE32(PINMUX_REG_BASE, REG_PU_2, Regpu);
			HAL_WRITE32(PINMUX_REG_BASE, REG_PD_2, Regpd);
		} else if (PullType == GPIO_PuPd_UP) {
			Regpu |= BIT(PinNum);
			Regpd &= ~(BIT(PinNum));

			HAL_WRITE32(PINMUX_REG_BASE, REG_PD_2, Regpd);
			HAL_WRITE32(PINMUX_REG_BASE, REG_PU_2, Regpu);
		} else if (PullType == GPIO_PuPd_NOPULL) {
			Regpu &= ~(BIT(PinNum));
			Regpd &= ~(BIT(PinNum));

			HAL_WRITE32(PINMUX_REG_BASE, REG_PU_2, Regpu);
			HAL_WRITE32(PINMUX_REG_BASE, REG_PD_2, Regpd);
		}
	}
}


/**
  *  @brief Set the function type of one pin.
  *  @param PinName : value of @ref PINMUX_Pin_Name_definitions.
  *  @param PinFunc : the function type for the pin, value of @ref PINMUX_Function_definitions.
  *  @retval None
  */
void Pinmux_Config(u8 PinName, u32 PinFunc)
{
	u32 Temp = 0;

	/* get PADCTR */
	Temp = PINMUX->REG_GPIOx[PinName];

	/* set needs function */
	Temp &= ~PAD_MASK_GPIOx_SEL;
	Temp |= PAD_GPIOx_SEL(PinFunc);

	/* set PADCTR register */
	PINMUX->REG_GPIOx[PinName] = Temp;
}


/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
