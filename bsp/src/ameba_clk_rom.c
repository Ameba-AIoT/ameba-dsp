/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

static const u32 XTAL_CLK[] = {
	40000000, /* EFUSE 0000: default */
	25000000, /* EFUSE 0001: */
	13000000, /* EFUSE 0010: */
	19200000, /* EFUSE 0011: */
	20000000, /* EFUSE 0100: */
	26000000, /* EFUSE 0101: */
	38400000, /* EFUSE 0110: */
	17664000, /* EFUSE 0111: */
	16000000, /* EFUSE 1000: */
	14318000, /* EFUSE 1001: */
	12000000, /* EFUSE 1010: */
	52000000, /* EFUSE 1011: */
	48000000, /* EFUSE 1100: */
	27000000, /* EFUSE 1101: */
	24000000, /* EFUSE 1110: */
	40000000, /* EFUSE 1111: */
};

/**
  * @brief  Get XTAL clock.
  * @retval  can be one of the following values:
  *		 @arg 40000000
  *		 @arg 25000000
  *		 @arg 13000000
  *		 @arg 19200000
  *		 @arg 20000000
  *		 @arg 26000000
  *		 @arg 38400000
  *		 @arg 17664000
  *		 @arg 16000000
  *		 @arg 14318000
  *		 @arg 12000000
  *		 @arg 52000000
  *		 @arg 48000000
  *		 @arg 26000000
  *		 @arg 27000000
  *		 @arg 24000000
  */
u32 XTAL_ClkGet(void)
{
	u32 clock_index = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_CKSL_GRP1);

	clock_index = LSYS_GET_CKSL_XTAL(clock_index);

	return XTAL_CLK[clock_index];
}



/**
  * @brief  Get PLL clock.
  * @retval  can be one of the following values:
  */
u32 PLL_ClkGet(u8 pll_type)
{
	PLL_TypeDef *PLL = (PLL_TypeDef *)PLL_BASE;
	u32 Div, FoF, FoN;
	u32 CpuClk, XtalClk;

	if (pll_type == CLK_CPU_MPLL) {
		Div = PLL_GET_CPUPLL_DIVN_SDM(PLL->PLL_CPUPLL_CTRL1);
		FoF = PLL_GET_CPUPLL_F0F_SDM(PLL->PLL_CPUPLL_CTRL3);
		FoN = PLL_GET_CPUPLL_F0N_SDM(PLL->PLL_CPUPLL_CTRL3);
	} else if (pll_type == CLK_CPU_DPLL) {
		Div = PLL_GET_CPUPLL_DIVN_SDM(PLL->PLL_PERIPLL_CTRL1);
		FoF = PLL_GET_CPUPLL_F0F_SDM(PLL->PLL_PERIPLL_CTRL3);
		FoN = PLL_GET_CPUPLL_F0N_SDM(PLL->PLL_PERIPLL_CTRL3);
	} else {
		/* error param return zero */
		return 0;
	}

	XtalClk = XTAL_ClkGet();

	FoN = FoN * (XtalClk >> 3) ;
	FoF = FoF * (XtalClk >> 16);
	Div = (Div + 2) * XtalClk;

	CpuClk = Div + FoN + FoF;
	return CpuClk;
}



/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
