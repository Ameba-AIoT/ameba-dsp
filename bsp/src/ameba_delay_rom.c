/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"


/**
  * @brief  Delay x nops.
  * @param  count: nop number
  * @note for KM4(default): HF=YES MVE=YES, code in ITCM, 7 cycles per loop. For 300M CPU clock, one loop cost: (1/300)*7=0.023us
  * @note for KM4: HF=NO MVE=YES, code in ICache, 6 cycles per loop. For 300M CPU clock, one loop cost: (1/300)*6=0.02us
  * @note for KM4 FPGA: 50M CPU clock, one loop cost: (1/50)*7=0.14us
  * @note for KR4: 4 cycles per loop. For 300M CPU clock, one loop cost: (1/300)*4=0.13us
  */

__weak __attribute__((noinline))  void DelayNop(u32 count)
{
	__asm__ __volatile__ ("1: addi %0, %0, -1; bgeui %0, 2, 1b"
				: "+r" (count));
}



/**
  * @brief  Delay x us.
  * @param  us: delay time, unit is us
  * @note the maximum value of us is 4294967296
  */
__weak void DelayUs(u32 us)
{
	u32 WaitCount;

	u32 CpuClk;

	u32 Clk = PLL_ClkGet(CLK_CPU_DPLL);

	/* one loop = 5/CpuClk us in DSP */
	u32 Div = 5;

	u32 temp = HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG0);
	u32 CPUID = LSYS_GET_HW_CPU_ID(temp);
	u32 ChipType = LSYS_GET_HW_CHIP_TYPE(temp);
	

	if (ChipType == CHIP_TYPE_FPGA) {
		CpuClk = 40;
	} else {
		CpuClk = Clk/1000000;
	}

	//WaitCount = (u32)(((u64)us) * ((u64)CpuClk) / ((u64)Div));
	WaitCount = (u32)(((u64)us) * CpuClk / Div);


	if (WaitCount == 0) {
		return;
	}
	
	DelayNop(WaitCount);

	return;
}

/**
  * @brief  Delay x ms.
  * @param  ms: delay time, unit is ms
  * @note the maximum value of ms is (14316557 + 8)/1000 = 14316ms
  */
__weak void DelayMs(u32 ms)
{
	DelayUs(ms * 1000);
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
