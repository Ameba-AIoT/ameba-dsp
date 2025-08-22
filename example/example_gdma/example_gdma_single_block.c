/**
 * *****************************************************************************
 * @file    example_gdma_single_block.c
 * @author
 * @version V1.0.0
 * @date    2023-09-01
 * @brief  example for GDMA single-blcok useage
 * *****************************************************************************
 * @attention
 *
 * This module is a confidential and proprietary property of RealTek and
 * possession or use of this module requires written permission of RealTek.
 *
 * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
 * *****************************************************************************
 */

#include "ameba_soc.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define ALIGN(x) __attribute__((aligned(x)))
#define SRAM_BSS __attribute__((section(".sram_dsp.bss")))

#define DATA_SIZE     256
#define DMA_SRC_OFFSET  0
#define DMA_DST_OFFSET  0

static ALIGN(64) uint8_t TestBuf1[DATA_SIZE];
static SRAM_BSS ALIGN(64) uint8_t TestBuf2[DATA_SIZE];
static volatile uint8_t dma_done;

struct gdma_s {
	u8 index;
	u8 ch_num;
	IRQ_FUN user_cb;
	u32 user_cb_data;
};
typedef struct gdma_s gdma_t;
static gdma_t gdma;

static void gdma_dump_memory(u8 *src, u32 len)
{
	u32 index = 0;
	u32 display_num = 16;

	for (index = 0; (len - index) >= display_num; index++) {
		if (!index) {
			DiagPrintf("[%08x] ", src);
		} else if (index % display_num == 0) {
			DiagPrintf("\r\n[%08x] ", src + index);
		}
		DiagPrintf("%2x ", src[index]);
	}
	while ((len - index) > 0) {
		DiagPrintf("%2x ", src[index]);
		index++;
	}
	DiagPrintf("\n");
}

static void gdma_done_callback(void)
{
	DiagPrintf("DMA Copy Done!!\r\n");
	dma_done = 1;
}

static u32 gdma_memcpy_int(void *pData)
{
	u32 IsrTypeMap = 0;
	u8 *pSrcData = NULL, *pDstData = NULL;
	gdma_t *dma_obj = (gdma_t *)pData;

	pSrcData = TestBuf1;
	pDstData = TestBuf2;
	/* Clean Auto Reload Bit */
	GDMA_ChCleanAutoReload(dma_obj->index, dma_obj->ch_num, CLEAN_RELOAD_SRC_DST);

	/* Write the cached value back to memory */
	DCache_Invalidate((void *) TestBuf2, DATA_SIZE);

	for (int i = 0; i < DATA_SIZE; i++) {
		if (pSrcData[i] != pDstData[i]) {
			DiagPrintf("GDMA Copy Memory Compare Err, %d %x %x\r\n", i, TestBuf1[i + DMA_SRC_OFFSET], TestBuf2[i + DMA_DST_OFFSET]);
			DiagPrintf("GDMA Src addr= 0x%08x, and length = %d\r\n", (uint32_t)pSrcData, DATA_SIZE);
			gdma_dump_memory(pSrcData, DATA_SIZE);
			DiagPrintf("GDMA Dst addr= 0x%08x, and length = %d\r\n", (uint32_t)pDstData, DATA_SIZE);
			gdma_dump_memory(pDstData, DATA_SIZE);
			return 1;
		}
	}

	/* Clear Pending ISR */
	IsrTypeMap = GDMA_ClearINT(dma_obj->index, dma_obj->ch_num);

	if (IsrTypeMap & TransferType) {
		if (dma_obj->user_cb != NULL) {
			dma_obj->user_cb((VOID *)dma_obj->user_cb_data);
		}
		GDMA_ChnlFree(gdma.index, gdma.ch_num);
		DiagPrintf("DMA Copy Memory Compare OK!! \r\n");
	}
	return 0;
}

static void gdma_singleblock_transfer_task(void)
{
	int i;
	GDMA_InitTypeDef GDMA_InitStruct;
	u8 ch_num;

	//1. Configure struct basic parameters
	DiagPrintf("\n%s:\n", __FUNCTION__);
	gdma.index = 0;
	gdma.user_cb = (IRQ_FUN)gdma_done_callback;
	gdma.user_cb_data = (u32)(&gdma);
	//2. Clear flag and Register interrupt for channel x
	dma_done = 0;
	ch_num = GDMA_ChnlAlloc(gdma.index, (IRQ_FUN) gdma_memcpy_int, (u32)(&gdma), 3);

	if (0xFF == ch_num) {
		DiagPrintf("%s: Cannot allocate a GDMA Channel\n", __FUNCTION__);
		return;
	}
	gdma.ch_num = ch_num;

	//3. set buffer
	DiagPrintf("Src=0x%x ==> Dst=0x%x Len=%d\r\n", TestBuf1 + DMA_SRC_OFFSET, TestBuf2 + DMA_DST_OFFSET, DATA_SIZE);
	for (i = 0; i < DATA_SIZE; i++) {
		TestBuf1[i] = i;
	}
	DCache_CleanInvalidate((void *) TestBuf1, DATA_SIZE);
	memset(TestBuf2, 0xff, DATA_SIZE);
	DCache_CleanInvalidate((void *) TestBuf2, DATA_SIZE);

	//4. Configure GDMA parameters
	memset((void *)&GDMA_InitStruct, 0, sizeof(GDMA_InitTypeDef));
	GDMA_InitStruct.GDMA_Index = gdma.index;
	GDMA_InitStruct.GDMA_ChNum = gdma.ch_num;
	GDMA_InitStruct.GDMA_DIR    = TTFCMemToMem;
	GDMA_InitStruct.GDMA_DstInc = IncType;
	GDMA_InitStruct.GDMA_SrcInc = IncType;
	GDMA_InitStruct.GDMA_IsrType = (TransferType | ErrType);

	if ((((u32)(TestBuf1 + DMA_SRC_OFFSET) & 0x03) == 0) &&	(((u32)(TestBuf2 + DMA_DST_OFFSET) & 0x03) == 0) && ((DATA_SIZE & 0x03) == 0)) {
		/* 4-bytes aligned, move 4 bytes each transfer */
		GDMA_InitStruct.GDMA_SrcMsize   = MsizeEight;
		GDMA_InitStruct.GDMA_SrcDataWidth = TrWidthFourBytes;
		GDMA_InitStruct.GDMA_DstMsize = MsizeEight;
		GDMA_InitStruct.GDMA_DstDataWidth = TrWidthFourBytes;
		GDMA_InitStruct.GDMA_BlockSize = DATA_SIZE >> TrWidthFourBytes;
	} else {
		GDMA_InitStruct.GDMA_SrcMsize   = MsizeEight;
		GDMA_InitStruct.GDMA_SrcDataWidth = TrWidthOneByte;
		GDMA_InitStruct.GDMA_DstMsize = MsizeEight;
		GDMA_InitStruct.GDMA_DstDataWidth = TrWidthOneByte;
		GDMA_InitStruct.GDMA_BlockSize = DATA_SIZE;
	}
	GDMA_InitStruct.GDMA_SrcAddr = (u32)(TestBuf1 + DMA_SRC_OFFSET);
	GDMA_InitStruct.GDMA_DstAddr = (u32)(TestBuf2 + DMA_DST_OFFSET);

	DiagPrintf("GDMA_InitStruct->GDMA_BlockSize = %d\n", GDMA_InitStruct.GDMA_BlockSize);

	GDMA_Init(GDMA_InitStruct.GDMA_Index, GDMA_InitStruct.GDMA_ChNum, &GDMA_InitStruct);
	GDMA_Cmd(GDMA_InitStruct.GDMA_Index, GDMA_InitStruct.GDMA_ChNum, ENABLE);

	//5. Wait for the GDMA transfer to complete
	while (dma_done == 0);
	while (1);
}

void gdma_singleblock_transfer_test(void)
{
	if (xTaskCreate((TaskFunction_t)gdma_singleblock_transfer_task, "GDMA SINGLE BLOCK TRANSFER DEMO", (2048 / 4), NULL, (tskIDLE_PRIORITY + 1),
					NULL) != pdPASS) {
		DiagPrintf("Cannot create dma_singleblock_task demo task\n\r");
	}
}
