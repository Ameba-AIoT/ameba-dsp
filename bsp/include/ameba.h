/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HAL_AMEBA_H_
#define _HAL_AMEBA_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dsp_wrapper.h"
#include "hal_platform.h"
#include <xtensa/hal.h>
#include "xtensa_api.h"
#include "ameba_vector.h"
#include "ameba_gdma.h"
#include "sysreg.h"
#include "ameba_pwmtimer.h"
#include "ameba_ipc.h"
#include "ameba_rcc.h"
#include "ameba_ipc_api.h"
#include "ameba_gpio.h"
#include "ameba_pinmux.h"
#include "ameba_delay.h"
#include "ameba_sport.h"
#include "ameba_audio.h"
#include "ameba_wdg.h"
#include "loguart.h"
#include "shell_cmd.h"
#include "sysreg_pll.h"
#include "ameba_clk.h"
#include "boot_load_dtcm.h"
#include "sysreg_pmc.h"
#include "ameba_pmc.h"
#define USE_FULL_ASSERT

#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0 : io_assert_failed((uint8_t *)__FUNCTION__, __LINE__))
/* Exported functions ------------------------------------------------------- */
__STATIC_INLINE void io_assert_failed(uint8_t *file, uint32_t line)
{
	DBG_8195A("io driver parameters error! file_name: %s, line: %d\n", file, line);

	for (;;);
}
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */


#endif //_HAL_AMEBA_H_
