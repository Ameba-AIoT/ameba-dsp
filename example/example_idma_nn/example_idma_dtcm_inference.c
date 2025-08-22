
/*
 *  Copyright (c) 2022 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 *
 * iDMA with NN compute example: 2022.9.2
 *
 * Note: you have to link "libxa_nnlib.a"
*/
#include "ameba_soc.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdint.h>
#include <stdlib.h>
#include <xtensa/idma.h>

#define NUM_MAX_DESC 2

#define ALIGN(x) __attribute__((aligned(x)))
#define DRAM0 __attribute__((section(".dram0.data")))
#define DRAM1 __attribute__((section(".dram1.data")))

#define WEIGHT_SIZE (512*768)
#define OUTPUT_SIZE 512
#define INPUT_SIZE  768
#define FETCH_SIZE  (128*768)  // iDMA one transport size
#define CONVERSION  1000 * 1000 / 1024 / 1024

int8_t ALIGN(128) fc_weights_psram[WEIGHT_SIZE];
int32_t ALIGN(16) DRAM0 fc_bias[OUTPUT_SIZE];
int8_t ALIGN(16) DRAM1 fc_input[INPUT_SIZE];
int8_t ALIGN(16) DRAM0 fc_output_idma[OUTPUT_SIZE];
int8_t ALIGN(16) DRAM0 fc_output_psram[OUTPUT_SIZE];

int8_t ALIGN(16) DRAM0 fc_weights_fraction_ping[FETCH_SIZE];
int8_t ALIGN(16) DRAM1 fc_weights_fraction_pong[FETCH_SIZE];

IDMA_BUFFER_DEFINE(BUFFER_IDMA, NUM_MAX_DESC, IDMA_1D_DESC);  // can't be static


void dtcm_idma_wr_thread(void *param)
{
	// Avoid Systick interference, which is more accurate for calculating time and speed
	taskENTER_CRITICAL();

	int ws = WEIGHT_SIZE;
	int fs = FETCH_SIZE;
	printf("INTO dtcm_idma_wr_thread.\n WEIGHT_SIZE:%d, FETCH_SIZE:%d\n", ws, fs);
	int fetch_times = ws / fs;
	printf("idma fetch times: %d \n", fetch_times);
	// init imputs and weight matrix with random numbers
	unsigned int seed = 10;
	srand(seed);
	for (int i = 0; i < INPUT_SIZE; i++) {
		fc_input[i] = rand() % 256;
	}
	for (int i = 0; i < 512 * 768; i++) {
		fc_weights_psram[i] = rand() % 256;
	}
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		fc_output_idma[i] = 59;
		fc_output_psram[i] = 60;
	}

	// Before using DMA, make sure that the data in the cache has been stored in RAM
	DCache_Clean((void *)fc_weights_psram, ws);

	uint32_t t0 = 0, t1 = 0, ca_t0 = 0, ca_t1 = 0, t_idma = 0;
	t0 = xthal_get_ccount();
	memcpy((void *)fc_weights_fraction_ping, (void *) & (fc_weights_psram[0 * fs]), fs);
	t1 = xthal_get_ccount();
	double dsp_cpy_time = (t1 - t0) / 500; // DSP run in 500Mhz
	double dsp_cpy_speed =  fs / dsp_cpy_time * CONVERSION;
	printf("dsp memcpy size: %d, cycle taken: %d, speed: %f MByte/s\n", fs, t1 - t0, dsp_cpy_speed);
	// *********************************************************** idma ping_pong *****************************************************************************
	// Initialize iDMAlib and iDMA hardware.
	idma_init(0, MAX_BLOCK_16, 16, TICK_CYCLES_1, 0, NULL);
	// Initialize a buffer and set the iDMAlib to fixed-buffer mode. The function initializes the task and organizes it as a circular buffer.
	idma_init_loop(BUFFER_IDMA, IDMA_1D_DESC, NUM_MAX_DESC, NULL, NULL);
	// start idma transfer: psram -> fc_weights_fraction_ping

	t0 = xthal_get_ccount();
	idma_copy_desc((void *)fc_weights_fraction_ping, (void *) & (fc_weights_psram[0 * fs]), fs, 0);
	while (idma_buffer_status() > 0)	{}
	t_idma = xthal_get_ccount();
	for (int i = 0; i < fetch_times - 1; i += 2) {
		// Wait idma fc_weights_fraction_ping write finish
		while (idma_buffer_status() > 0)	{}
		// start to fill pong buf with idma
		idma_copy_desc((void *)fc_weights_fraction_pong, (void *) & (fc_weights_psram[(i + 1)*fs]), fs, 0);
		// compute fc_weights_fraction_ping (fully connected compute)
		ca_t0 = xthal_get_ccount();
		xa_nn_fully_connected_sym8sxasym8s_asym8s((signed char *)&fc_output_idma[i * OUTPUT_SIZE / fetch_times], fc_weights_fraction_ping, fc_input,
				(int32_t *)&fc_bias[i * OUTPUT_SIZE / fetch_times], INPUT_SIZE, 128, 0, 1776441054, -9, -128);
		ca_t1 = xthal_get_ccount();
		// Wait idma fc_weights_fraction_pong write finish
		while (idma_buffer_status() > 0)	{}
		// The last cycle does not fetch data
		if (i != fetch_times - 2) {
			// start idma transfer: psram -> fc_weights_fraction_ping
			idma_copy_desc((void *)fc_weights_fraction_ping, (void *) & (fc_weights_psram[(i + 2) * fs]), fs, 0);
		}
		// compute fc_weights_fraction_ping (fully connected compute)
		xa_nn_fully_connected_sym8sxasym8s_asym8s((signed char *)&fc_output_idma[(i + 1) * OUTPUT_SIZE / fetch_times], fc_weights_fraction_pong, fc_input,
				(int32_t *)&fc_bias[(i + 1) * OUTPUT_SIZE / fetch_times], INPUT_SIZE, 128, 0, 1776441054, -9, -128);
	}
	t1 = xthal_get_ccount();
	double idma_time = (t_idma - t0) / 500;
	double idma_speed =  fs / idma_time * CONVERSION;
	printf("iDMA tranfer size: %d, cycle taken: %d, speed: %f MByte/s\n", fs, t_idma - t0, idma_speed);
	printf("fc caculation time:%d\n", ca_t1 - ca_t0);
	printf("cacultaion with iDMA time: %d cycles\n", t1 - t0);

	// *********************************************************** psram caculation *****************************************************************************
	DCache_CleanInvalidate(fc_weights_psram, WEIGHT_SIZE);
	t0 = xthal_get_ccount();
	xa_nn_fully_connected_sym8sxasym8s_asym8s((signed char *)&fc_output_psram[0], fc_weights_psram, fc_input,
			(int32_t *)&fc_bias[0], INPUT_SIZE, 512, 0, 1776441054, -9, -128);
	t1 = xthal_get_ccount();
	taskEXIT_CRITICAL();
	printf("cacultaion in psram time: %d cycles\n", t1 - t0);

	// *********************************************************** compare ***********************************************************
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		if (fc_output_idma[i] != fc_output_psram[i]) {
			printf("result error: fc_output_idma[%d]=%d, fc_output_psram[%d]=%d \n", i, fc_output_idma[i], i, fc_output_psram[i]);
			while (1);
		}
	}
	printf("iDMA fetch and psram result is same ! \n");

	while (1) {
		vTaskDelay(1000);
	}
}

void example_idma_inference(void)
{
	if (xTaskCreate(dtcm_idma_wr_thread, ((const char *)"dtcm_idma_wr_thread"), 2048, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		printf("\n\r%s xTaskCreate failed", __FUNCTION__);
	}
}
