/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"


void vAssertCalled(unsigned long ulLine, const char *const pcFileName);


/**
 * Prototypes for the standard FreeRTOS application hook (callback) functions
 * implemented within this file.
 *
 * @see http://www.freertos.org/a00016.html
 */
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationTickHook(void);

/**
 * The function called from the tick hook.
 *
 * @note Only the comprehensive demo uses application hook (callback) functions.
 *
 * @see http://www.freertos.org/a00016.html
 */
void vFullDemoTickHookFunction(void);
/*-----------------------------------------------------------*/



/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	 * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
	 * function that will get called if a call to pvPortMalloc() fails.
	 * pvPortMalloc() is called internally by the kernel whenever a task, queue,
	 * timer or semaphore is created.  It is also called by various parts of the
	 * demo application.  If heap_1.c, heap_2.c or heap_4.c is being used, then
	 * the size of the	heap available to pvPortMalloc() is defined by
	 * configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
	 * API function can be used to query the size of free heap space that remains
	 * (although it does not provide information on how the remaining heap might be
	 * fragmented). See http://www.freertos.org/a00111.html for more information.
	 */
	char *pcCurrentTask = "NoTsk";
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
		pcCurrentTask = pcTaskGetName(NULL);
	}
	printf("[%s]DSP Malloc failed [free heap size: %d]\r\n",  pcCurrentTask, xPortGetFreeHeapSize());

	vAssertCalled(__LINE__, __FILE__);
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	 * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	 * function is called if a stack overflow is detected. */
	printf("DSP task STACK OVERFLOW - TaskName(%s)\n\r", pcTaskName);

	vAssertCalled(__LINE__, __FILE__);
}
/*-----------------------------------------------------------*/
void vFullDemoTickHookFunction()
{

}

void vApplicationTickHook(void)
{
	/* This function will be called by each tick interrupt if
	 * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	 * added here, but the tick hook is called from an interrupt context, so
	 * code must not attempt to block, and only the interrupt safe FreeRTOS API
	 * functions can be used (those that end in FromISR()). */
#if ( mainCREATE_SIMPLE_BLINKY_DEMO_ONLY != 1 )
	{
		vFullDemoTickHookFunction();
	}

#endif /* mainCREATE_SIMPLE_BLINKY_DEMO_ONLY */
}
/*-----------------------------------------------------------*/

void vAssertCalled(unsigned long ulLine, const char *const pcFileName)
{
	static BaseType_t xPrinted = pdFALSE;
	volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;

	/* Called if an assertion passed to configASSERT() fails.  See
	 * http://www.freertos.org/a00110.html#configASSERT for more information. */

	/* Parameters are not used. */
	(void) ulLine;
	(void) pcFileName;

	taskENTER_CRITICAL();
	{
		/* You can step out of this function to debug the assertion by using
		 * the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
		 * value. */
		while (ulSetToNonZeroInDebuggerToContinue == 0) {
			__asm volatile("NOP");
			__asm volatile("NOP");
		}
	}
	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

__attribute__((weak)) void app_example(void)
{

}

int main()
{
	Boot_LoadDTCM();

	printf("hello dsp world!\n");

	shell_cmd_init();

	app_example();

	vTaskStartScheduler();
}
