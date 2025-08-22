/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_freertos_pmu.h"
#include "ameba_soc.h"

unsigned int missing_tick = 0;

static unsigned int wakelock     = DEFAULT_WAKELOCK;
static unsigned int sleepwakelock_timeout     = 0;
static u32 system_can_yield = 1; /* default is can */
static unsigned int sleep_type = SLEEP_CG; /* 0 is power gate, 1 is clock gate */
static unsigned int max_sleep_time = 0; /* if user want wakeup peridically, can set this timer*/
SLEEP_ParamDef sleep_param __attribute__((aligned(32)));  /* cacheline aligned */

static unsigned int sysactive_timeout_temp = 0;
static unsigned int sysactive_timeout_flag = 0;

u32 tickless_debug = 0;

/* ++++++++ FreeRTOS macro implementation ++++++++ */


#define SYSTICK_THRES 0x7fffffff
/*
return: TRUE: time1 > time2
*/
static int freertos_systick_check(u32 time1, u32 time2)
{
	u32 delta = time1 > time2 ? time1 - time2 : time2 - time1;
	if (delta < SYSTICK_THRES) {
		return time1 >= time2 ? true : false;
	} else {	//overflow
		return time1 <= time2 ? true : false;
	}
}


unsigned int pmu_set_sysactive_time(unsigned int timeout)
{
	u32 TimeOut = 0;

	if (sysactive_timeout_flag) {
		if (timeout > sysactive_timeout_temp) {
			sysactive_timeout_temp = timeout;
		}
		return 1;
	}

	if (sysactive_timeout_temp > timeout) {
		timeout = sysactive_timeout_temp;
	}
	sysactive_timeout_temp = 0;

	TimeOut = xTaskGetTickCount() + timeout;

	if (freertos_systick_check(TimeOut, sleepwakelock_timeout)) {
		sleepwakelock_timeout = TimeOut;
	}

	return 0;
}

/* can not yield CPU under suspend/resume process */
unsigned int pmu_yield_os_check(void)
{
	return system_can_yield;
}

unsigned int ccompare_ccount_delta_bk;

void pmu_xt_timer_cmd(unsigned int enable)
{
	if (enable) {
		/* enable xt timer interrupt, and restore ccompare and ccount reg */
		unsigned int ccount = xt_get_ccount();
		unsigned int ccompare_new = ccount + ccompare_ccount_delta_bk;
		if (ccompare_new < ccount) {
			ccompare_new = 0xffffffff;
		}
		xt_set_ccompare(XT_TIMER_INDEX, ccompare_new);
		xt_interrupt_enable(XT_TIMER_INTNUM);
	} else {
		/* disable xt timer interrupt, and backup ccompare and ccount reg */
		xt_interrupt_disable(XT_TIMER_INTNUM);
		unsigned int ccompare = xt_get_ccompare(XT_TIMER_INDEX);
		unsigned int ccount = xt_get_ccount();
		if (ccompare > ccount) {
			ccompare_ccount_delta_bk = ccompare - ccount;
		} else {
			ccompare_ccount_delta_bk = 1;
		}
		xt_set_ccompare(XT_TIMER_INDEX, 0);
	}
}

/*
 *  It is called in idle task.
 *
 *  @return  true  : System is ready to check conditions that if it can enter sleep.
 *           false : System keep awake.
 **/
int freertos_ready_to_sleep(void)
{
	u32 current_tick = xTaskGetTickCountFromISR();

	//DiagPrintf("wakelock: %d\n", wakelock);
	/* timeout */
	if (freertos_systick_check(current_tick, sleepwakelock_timeout) == false) {
		return false;
	}

	if (wakelock == 0) {
		return true;
	} else {
		return false;
	}
}


/*
 *  It is called when freertos is going to sleep.
 *  At this moment, all sleep conditons are satisfied. All freertos' sleep pre-processing are done.
 *
 *  @param  expected_idle_time : The time that FreeRTOS expect to sleep.
 *                               If we set this value to 0 then FreeRTOS will do nothing in its sleep function.
 **/
void freertos_pre_sleep_processing(unsigned int *expected_idle_time)
{
	unsigned int tick_before_sleep;
	unsigned int tick_passed;
	volatile unsigned int ms_passed = 0;

	sleep_param.sleep_time = max_sleep_time;//*expected_idle_time;
	max_sleep_time = 0;
	sleep_param.dlps_enable = DISABLE;

	sleep_param.sleep_type = sleep_type;

	*expected_idle_time = 0;

	/*  Store gtimer timestamp before sleep */
	tick_before_sleep = SYSTIMER_TickGet();
	sysactive_timeout_flag = 1;

	SOCPS_SleepCG();

	/*  update kernel tick by calculating passed tick from gtimer */
	/*  get current gtimer timestamp */
	tick_passed = SYSTIMER_GetPassTick(tick_before_sleep);

	tick_passed += missing_tick;
	missing_tick = tick_passed & 0x1F;

	/* ms =x*1000/32768 = (x *1000) >>15 */
	ms_passed = (u32)(((u64)(tick_passed) * 1000) >> 15);		//u64 need check

	vTaskStepTick(ms_passed); /*  update kernel tick */

	sysactive_timeout_flag = 0;
	sleepwakelock_timeout = xTaskGetTickCountFromISR() + 1;

	if (tickless_debug) {
		DBG_8195A("dsp sleeped:[%d] ms\n", ms_passed);
	}

}

void freertos_post_sleep_processing(unsigned int *expected_idle_time)
{
	(void)expected_idle_time;
}

void vPortSuppressTicksAndSleep(TickType_t target, TickType_t xExpectedIdleTime)
{
	unsigned int xModifiableIdleTime;
	eSleepModeStatus eSleepStatus;
	unsigned int ps;

	system_can_yield = 0;

	// Lock out all interrupts. Otherwise reading and using ccount can
	// get messy. Shouldn't be a problem here since we are about to go
	// to sleep, and the waiti will re-enable interrupts shortly.
	pmu_xt_timer_cmd(DISABLE);
	ps = portENTER_CRITICAL_NESTED();

	eSleepStatus = eTaskConfirmSleepModeStatus();
	if (eSleepStatus == eAbortSleep) {
		// Abort, fall through.
	} else if (freertos_ready_to_sleep()) {
		/* Sleep until something happens.  configPRE_SLEEP_PROCESSING() can
		set its parameter to 0 to indicate that its implementation contains
		its own wait for interrupt or wait for event instruction, and so wfi
		should not be executed again.  However, the original expected idle
		time variable must remain unmodified, so a copy is taken. */
		xModifiableIdleTime = (unsigned int)xExpectedIdleTime;
		configPRE_SLEEP_PROCESSING(xModifiableIdleTime);
		if (xModifiableIdleTime > 0) {
			XT_ISYNC();
			XT_WAITI(0);
		}
		configPOST_SLEEP_PROCESSING(xModifiableIdleTime);
	} else {
		pmu_xt_timer_cmd(ENABLE);
		vPortSuppressTicksAndSleep_tickless(target, xExpectedIdleTime);
	}

	portEXIT_CRITICAL_NESTED(ps);
	pmu_xt_timer_cmd(ENABLE);

	system_can_yield = 1;
}

/* -------- FreeRTOS macro implementation -------- */

void pmu_acquire_wakelock(unsigned int nDeviceId)
{
	wakelock |= BIT(nDeviceId);
}

void pmu_release_wakelock(unsigned int nDeviceId)
{
	wakelock &= ~BIT(nDeviceId);
}

unsigned int pmu_get_wakelock_status(void)
{
	return wakelock;
}

unsigned int pmu_set_sleep_type(unsigned int type)
{
	sleep_type = type;

	return 0;
}

unsigned int pmu_get_sleep_type(void)
{
	return sleep_type;
}

void pmu_set_max_sleep_time(unsigned int timer_ms)
{
	max_sleep_time = timer_ms;
}


void pmu_tickless_debug(u32 NewStatus)
{
	if (NewStatus == ENABLE) {
		tickless_debug = 1;
	} else {
		tickless_debug = 0;
	}
}
