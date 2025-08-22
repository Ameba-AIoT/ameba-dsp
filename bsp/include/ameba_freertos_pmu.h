/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _FREERTOS_PMU_8721D_H_
#define _FREERTOS_PMU_8721D_H_

typedef enum {
	PMU_OS				= 0,

	PMU_MAX				= 31
} PMU_DEVICE;

enum SLEEP_TYPE {
	SLEEP_PG	= 0,
	SLEEP_CG	= 1,
};

// default locked by OS and not to sleep until OS release wakelock in somewhere
#define DEFAULT_WAKELOCK		(BIT(PMU_OS))
#define SLEEP_MAX_DELAY		(u32) 0xffffffffUL

/**
  * @brief  init system active timer for PMU.
  * @param  none.
  * @retval status value:
  *          - 0: _FAIL
  *          - 1: _SUCCESS
  * @note can just used in late resume or later, can not used in wakeup_hook_fun.
  */
unsigned int pmu_yield_os_check(void);
unsigned int pmu_set_sleep_type(unsigned int type);
unsigned int pmu_get_sleep_type(void);
unsigned int pmu_set_sysactive_time(unsigned int timeout);
void pmu_set_max_sleep_time(unsigned int timer_ms);

void pmu_tickless_debug(unsigned int NewStatus);

void pmu_xt_timer_cmd(unsigned int enable);

int freertos_ready_to_sleep(void);
void freertos_pre_sleep_processing(unsigned int *expected_idle_time);
void freertos_post_sleep_processing(unsigned int *expected_idle_time);

void pmu_acquire_wakelock(unsigned int nDeviceId);
void pmu_release_wakelock(unsigned int nDeviceId);
unsigned int pmu_get_wakelock_status(void);

extern unsigned int tickless_debug;
#endif
