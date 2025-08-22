/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ameba_soc.h"
#include "FreeRTOS.h"
#include "task.h"

extern unsigned char _command_start[];
extern unsigned char _command_end[];

#if ( ( configGENERATE_RUN_TIME_STATS == 1 ) && ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

static TaskHandle_t task_status_log_task_Handler = NULL;
static char InfoBuffer[1024];
static char InfoBuffer2[1024 * 2];
static unsigned long time_delay;

static char *prvWriteNameToBuffer(char *pcBuffer, const char *pcTaskName)
{
	size_t x;

	/* Start by copying the entire string. */
	strcpy(pcBuffer, pcTaskName);

	/* Pad the end of the string with spaces to ensure columns line up when printed out. */
	for (x = strlen(pcBuffer); x < (size_t)(configMAX_TASK_NAME_LEN - 1); x++)	{
		pcBuffer[ x ] = ' ';
	}

	/* Terminate. */
	pcBuffer[ x ] = (char) 0x00;

	/* Return the new end of string. */
	return &(pcBuffer[ x ]);
}

void vTaskGetRunTimeStats_rtk(char *pcWriteBuffer)
{
	TaskStatus_t *pxTaskStatusArray_old;
	TaskStatus_t *pxTaskStatusArray_new;
	UBaseType_t uxArraySize, x1, x2;
	UBaseType_t uxArraySize_old;
	UBaseType_t uxArraySize_new;
	float ulTotalTime, ulStatsAsPercentage;
	uint32_t ulTotalTime_old;
	uint32_t ulTotalTime_new;

	/* Make sure the write buffer does not contain a string. */
	*pcWriteBuffer = (char) 0x00;

	/* Take a snapshot of the number of tasks in case it changes while this function is executing. */
	uxArraySize_old = uxTaskGetNumberOfTasks();

	/* If configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will equate to NULL. */
	pxTaskStatusArray_old = pvPortMalloc(uxArraySize_old * sizeof(TaskStatus_t));

	if (pxTaskStatusArray_old != NULL) {
		uxArraySize_old = uxTaskGetSystemState(pxTaskStatusArray_old, uxArraySize_old, &ulTotalTime_old);
	} else {
		mtCOVERAGE_TEST_MARKER();
	}

	vTaskDelay(time_delay * 1000);

	/* Take a snapshot of the number of tasks in case it changes while this function is executing. */
	uxArraySize_new = uxTaskGetNumberOfTasks();

	/* If configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will equate to NULL. */
	pxTaskStatusArray_new = pvPortMalloc(uxArraySize_new * sizeof(TaskStatus_t));

	if (pxTaskStatusArray_new != NULL) {
		uxArraySize_new = uxTaskGetSystemState(pxTaskStatusArray_new, uxArraySize_new, &ulTotalTime_new);
	} else {
		mtCOVERAGE_TEST_MARKER();
	}

	/* For percentage calculations. */
	ulTotalTime = (float)((ulTotalTime_new - ulTotalTime_old));
	/* Avoid divide by zero errors. */
	if (ulTotalTime > 0UL) {
		/* Create a human readable table from the binary data. */
		for (x1 = 0; x1 < uxArraySize_new; x1++) {
			for (x2 = 0; x2 < uxArraySize_old; x2++) {
				if (pxTaskStatusArray_new[x1].xTaskNumber == pxTaskStatusArray_old[x2].xTaskNumber) {
					ulStatsAsPercentage = (float)(pxTaskStatusArray_new[ x1 ].ulRunTimeCounter - pxTaskStatusArray_old[ x2 ].ulRunTimeCounter) / ulTotalTime * 100;
					pcWriteBuffer = prvWriteNameToBuffer(pcWriteBuffer, pxTaskStatusArray_new[ x1 ].pcTaskName);
					sprintf(pcWriteBuffer, "\t%u\t%-10u \t%8.2f%% \r\n", (unsigned int) pxTaskStatusArray_new[ x1 ].uxCurrentPriority,
							((unsigned int) pxTaskStatusArray_new[ x1 ].ulRunTimeCounter - pxTaskStatusArray_old[ x2 ].ulRunTimeCounter),  ulStatsAsPercentage);
					pcWriteBuffer += strlen(pcWriteBuffer);
					break;
				}
			}
		}
	} else {
		mtCOVERAGE_TEST_MARKER();
	}

	vPortFree(pxTaskStatusArray_old);
	vPortFree(pxTaskStatusArray_new);
}

void task_status_log_task(void)
{
	while (1) {
		vTaskGetRunTimeStats_rtk(InfoBuffer);
		DiagPrintf("[DSP] FreeRTOS FreeHeapSize: %d \n", xPortGetFreeHeapSize());
		vTaskList(InfoBuffer2);
		DiagPrintf("[DSP] Name              State  Priority  Stack  Num \n");
		DiagPrintf("%s \r\n", InfoBuffer2);
		DiagPrintf("[DSP] Name             Priority  DeltaRunTime(us)  Percentage(total:100%%)\r\n");
		DiagPrintf("%s\r\n", InfoBuffer);
	}
}

u32 task_status(u16 argc, u8 *argv[])
{
	if (argc < 1) {
		DiagPrintf("need more argument. e.g.  start, stop \n");
		return 1;
	}

	if (!strcmp((const char *)argv[0], "start")) {
		if (argc < 2) {
			DiagPrintf("need time argument. e.g. \"status start 1\" \n");
			return 1;
		} else {
			time_delay = strtoul((const char *)(argv[1]), (char **)NULL, 10);
		}
		if (!task_status_log_task_Handler) {
			xTaskCreate((TaskFunction_t)task_status_log_task,
						(const char *)"task_status_log_task",
						(uint16_t)1024,
						(void *)NULL,
						(UBaseType_t)2,
						(TaskHandle_t *)&task_status_log_task_Handler);
		} else {
			DiagPrintf("[DSP][monitor.c] task_status_log_task task is already running.\n");
		}
	} else if (!strcmp((const char *)argv[0], "stop")) {
		if (task_status_log_task_Handler) {
			vTaskDelete(task_status_log_task_Handler);
			task_status_log_task_Handler = NULL;
		} else {
			DiagPrintf("[DSP][monitor.c] task_status_log_task task is NOT running.\n");
		}
	} else if (argc) {
		DiagPrintf("invalid argument: %s \n", argv[0]);
		return 1;
	}
	return 0;
}

__attribute__((section(".command"))) const COMMAND_TABLE monitor_status_table[] = {
	{
		(const u8 *)"status",  4, task_status, (const u8 *)"\tstatus \n"
		"\t\t status start <period in second>: start tasks status log,  out put statistics of cpu usage over a period of time  \n"
		"\t\t status stop:  stop tasks status log \n"
	},
};


#endif

u32 dsp_tickps(u16 argc, u8 *argv[])
{
	if (argc < 1) {
		DiagPrintf("need more argument. e.g. start, stop \n");
		return 1;
	}

	if (!strcmp((const char *)argv[0], "r")) {
		pmu_release_wakelock(PMU_OS);
		if (argc >= 2 && strcmp((const char *)argv[1], "debug") == 0) {
			pmu_tickless_debug(ENABLE);
		} else {
			pmu_tickless_debug(DISABLE);
		}
	}

	if (!strcmp((const char *)argv[0], "a")) {
		pmu_acquire_wakelock(PMU_OS);
	}

	if (!strcmp((const char *)argv[0], "get")) {
		DiagPrintf("lockbit:%x \n", pmu_get_wakelock_status());
	}

	if (!strcmp((const char *)argv[0], "type")) {
		if (argc >= 2 && strcmp((const char *)argv[1], "pg") == 0) {
			pmu_set_sleep_type(SLEEP_PG);
		} else if (argc >= 2  && strcmp((const char *)argv[1], "cg") == 0) {
			pmu_set_sleep_type(SLEEP_CG);
		} else {
			pmu_set_sleep_type(SLEEP_PG);
		}
	}

	return 0;
}

u32 CmdRamHelp(u16 argc, u8  *argv[])
{
	/* To avoid gcc warnings */
	(void) argc;
	(void) argv;

	PCOMMAND_TABLE cmd_start = (PCOMMAND_TABLE)(&_command_start);
	PCOMMAND_TABLE cmd_end = (PCOMMAND_TABLE)(&_command_end);
	int cmd_mum = (int)(((u32)cmd_end - (u32)cmd_start) / sizeof(COMMAND_TABLE));
	u32	index;

	DiagPrintf("----------------- DSP TEST COMMAND MODE HELP %d [%p:%p]------------------\n",
			   cmd_mum, cmd_start, cmd_end);
	for (index = 0; index < cmd_mum; index++) {
		if (cmd_start[index].msg) {
			DiagPrintf("%s\n", cmd_start[index].msg);
		}
	}
	DiagPrintf("----------------- DSP TEST COMMAND MODE END  %d [%p:%p]------------------\n",
			   cmd_mum, cmd_start, cmd_end);

	return 0;
}

__attribute__((section(".command"))) const COMMAND_TABLE monitor_cmd_table[] = {
	{
		(const u8 *)"dsp_tickps",  4, dsp_tickps, (const u8 *)"\tdsp_tickps\n"
		"\t\t r: release os wakelock \n"
		"\t\t a: acquire os wakelock \n"
	},
	{
		(const u8 *)"?",		0, CmdRamHelp,	(const u8 *)"\tHELP (?) : \n"
		"\t\t Print help messag\n"
	},
};

