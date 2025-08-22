/*
 *  Copyright (c) 2022 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 *
 * iDMA example: 2022.9.1
 * Fixed-Buffer Mode
 * One-dimensional (1D)
 * 64KB, from PSRAM to DTCM
 *
 * refer to Xplorer Doc: sys_sw_rm.pdf: 8. The Integrated DMA Library API
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtensa/hal.h>
#include <stdint.h>
#include <xtensa/idma.h>

#include "ameba_soc.h"
#include "example_idma.h"


#define NUM_MAX_DESC 2
#define IDMA_CH_ARG_SINGLE

#define ALIGN(x) __attribute__((aligned(x)))
#define DRAM0 __attribute__((section(".dram0.data")))
#define DRAM1 __attribute__((section(".dram1.data")))

#define ENABLE_INT 1

#define DATA_SIZE 64*1024

static int8_t ALIGN(16) src_data[DATA_SIZE];  // psram
static int8_t DRAM1 dst_data[DATA_SIZE];  // dram

IDMA_BUFFER_DEFINE(buffer_idma, NUM_MAX_DESC, IDMA_1D_DESC);  // can not be static !!!

#if ENABLE_INT
static void idma_done_interrupt_handler(void)
{
	DiagPrintf("idma fetch finished \n");
}


static void idma_error_interrupt_handler(void)
{
	DiagPrintf("idma fetch error \n");
}
#endif

static void idmaErrorCallBack(const idma_error_details_t *data)
{
	DiagPrintf("ERROR CALLBACK: iDMA in Error\n");
	idma_error_details_t *error = idma_error_details(IDMA_CH_ARG_SINGLE);
	DiagPrintf("COPY FAILED, Error 0x%x at desc:%p, PIF src/dst=%x/%x\n", error->err_type, (void *)error->currDesc, error->srcAddr, error->dstAddr);
}


static void dtcm_idma_wr_thread(void *param)
{
	printf("INTO dtcm_idma_wr_thread.\n");

	for (int i = 0; i < DATA_SIZE; i ++)    {
		src_data[i] = rand() % 256;
	}
	// Before using IDMA, write the data in the cache to the source address
	DCache_Clean(src_data, DATA_SIZE);

	// Initialize iDMAlib and iDMA hardware
	idma_init(0, MAX_BLOCK_16, 16, TICK_CYCLES_1, 0, idmaErrorCallBack);

	// Initialize a buffer and set the iDMAlib to fixed-buffer mode.
	idma_init_loop(buffer_idma, IDMA_1D_DESC, NUM_MAX_DESC, NULL, NULL);

	uint32_t t0 = xthal_get_ccount();

#if ENABLE_INT
	idma_register_interrupts(0, (os_handler)idma_done_interrupt_handler, (os_handler)idma_error_interrupt_handler);
	// Update the next in line 1D descriptor and schedule it. idma transfer: psram -> dtcm1
	idma_copy_desc(dst_data, src_data, DATA_SIZE, DESC_NOTIFY_W_INT);  // enable idma done handler
#else
	idma_copy_desc(dst_data, src_data, DATA_SIZE, 0);
#endif
	// Get the buffer execution status by reading the number of outstanding descriptors (not yet completed ones)
	while (idma_buffer_status() > 0) { }
	idma_status_t ret = idma_buffer_status();
	if (ret < 0) {
		printf("iDMA transfer error. ERROR CODE:%d \n", ret);
	}

	uint32_t t1 = xthal_get_ccount();

	// Check whether the original data and target data are consistent
	for (size_t i = 0; i < DATA_SIZE; i++)	{
		if (src_data[i] != dst_data[i])		{
			printf("Data copy error: src_data[%d]:%d,  dst_data[%d]:%d \n", i, src_data[i], i, dst_data[i]);
			while (1);
		}
	}
	printf("Data copy finished. \n");
	printf("idma transfer time: %d cycles, DATA_SIZE:%d \n", t1 - t0, DATA_SIZE);

	// Avoid Systick interference, which is more accurate for calculating time and speed
	taskENTER_CRITICAL();
	t0 = xthal_get_ccount();
	memcpy(dst_data, src_data, DATA_SIZE);
	t1 = xthal_get_ccount();
	taskEXIT_CRITICAL();

	printf("dsp memcpy transfer time: %d cycles, DATA_SIZE:%d \n", t1 - t0, DATA_SIZE);

	while (1) {
		vTaskDelay(1000);
	}
}

void example_dtcm_idma_wr(void)
{
	if (xTaskCreate(dtcm_idma_wr_thread, ((const char *)"dtcm_idma_wr_thread"), 2048, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate failed", __FUNCTION__);
	}
}
