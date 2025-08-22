/**
 * *****************************************************************************
 * @file    example_gdma_multi_block.c
 * @author
 * @version V1.0.0
 * @date    2023-09-01
 * @brief  example for GDMA multi-blcok useage
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

#define GDMA_CHANNEL_ID 0
#define DATA_SIZE 		128
#define BLOCK_SIZE 		32
#define DMA_SRC_OFFSET  0
#define DMA_DST_OFFSET  0

enum {
	SingleBlock   = 1,
	ReloadDst     = 2,
	ReloadSrc     = 3,
	ReloadSrcDst  = 4,
	MAXGdmaTestMode
};

static u8 	GdmaType = ReloadDst;
static u32 	Gdma_BlockLen = 0;

struct gdma_s {
	u8 index;
	u8 ch_num;
	IRQ_FUN user_cb;
	u32 user_cb_data;
};
typedef struct gdma_s gdma_t;
static gdma_t gdma;

static ALIGN(64) uint8_t TestBuf1[DATA_SIZE];
static SRAM_BSS ALIGN(64) uint8_t TestBuf2[DATA_SIZE];
static volatile uint8_t dma_done;


static BOOL gdma_test_compare(u8 *pSrc, u8 *pDst, u32 PacketLen)
{
	u32 CheckIndex = 0;

	for (CheckIndex = 0; CheckIndex < PacketLen; CheckIndex++) {
		if (pSrc[CheckIndex] != pDst[CheckIndex]) {
			return -1;
		}
	}
	return 0;
}

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

static u32 gdma_multiblock_irq(VOID *Data)
{
	PGDMA_InitTypeDef GDMA_InitStruct = (PGDMA_InitTypeDef) Data;
	u8 *pSrcData = NULL, *pDstData = NULL;
	u32 IsrTypeMap = 0;
	u8 this_is_last_two = 0;

	if (GDMA_InitStruct->MaxMuliBlock == GDMA_InitStruct->MuliBlockCunt + 2) {
		this_is_last_two = 1;
	}

	// Clear Auto Reload Bit
	if (this_is_last_two) {
		GDMA_ChCleanAutoReload(0, GDMA_InitStruct->GDMA_ChNum, CLEAN_RELOAD_SRC_DST);
	}

	switch (GdmaType) {
	case  ReloadDst:
		pSrcData = (u8 *)(GDMA_InitStruct->GDMA_SrcAddr +
						  Gdma_BlockLen * (GDMA_InitStruct->MuliBlockCunt));
		pDstData = (u8 *)GDMA_InitStruct->GDMA_DstAddr ;
		break;
	case ReloadSrc:
		pSrcData = (u8 *)GDMA_InitStruct->GDMA_SrcAddr;
		pDstData = (u8 *)(GDMA_InitStruct->GDMA_DstAddr +
						  Gdma_BlockLen * (GDMA_InitStruct->MuliBlockCunt));
		break;
	case ReloadSrcDst:
		pSrcData = (u8 *)GDMA_InitStruct->GDMA_SrcAddr;
		pDstData = (u8 *)GDMA_InitStruct->GDMA_DstAddr;
		break;
	}

	/* reload cache from sram before use the DMA destination data */
	DCache_Invalidate((void *) pDstData, Gdma_BlockLen);

	//Compare Data should before HalGdmaChEn
	if (gdma_test_compare(pSrcData, pDstData, Gdma_BlockLen)) {
		DiagPrintf("GDMA Block %d Error: Check Data Error!\n", GDMA_InitStruct->MuliBlockCunt);
		DiagPrintf("GDMA Src addr= 0x%08x, and length = %d\r\n", (void *) pSrcData, BLOCK_SIZE);
		gdma_dump_memory(pSrcData, BLOCK_SIZE);
		DiagPrintf("GDMA Dst addr= 0x%08x, and length = %d\r\n", (void *) pDstData, BLOCK_SIZE);
		gdma_dump_memory(pDstData, BLOCK_SIZE);
	}

	//reset dest for next block compare
	memset(pDstData, 0x00, Gdma_BlockLen);
	DCache_Clean((void *) pDstData, Gdma_BlockLen);

	//Clear Pending ISR, next block will start transfer
	IsrTypeMap = GDMA_ClearINT(0, GDMA_InitStruct->GDMA_ChNum);

	//DiagPrintf("DMA INT %08x\n", IsrTypeMap);
	if (IsrTypeMap & BlockType) {
		DiagPrintf("DMA Block %d Transfer Complete\r\n\r\n", GDMA_InitStruct->MuliBlockCunt++);
	}

	//last IRQ is TransferType in multi-block
	if (IsrTypeMap & TransferType) {
		DiagPrintf("DMA Last Block Complete\n");
		DiagPrintf("DMA Data MaxMuliBlock = %d \n", GDMA_InitStruct->MaxMuliBlock);
		GDMA_ChnlFree(0, GDMA_InitStruct->GDMA_ChNum);
		dma_done = 1;
	}
	return 0;
}

static void gdma_multiblock_transfer_task(void)
{
	IRQn_Type   IrqNum;
	GDMA_InitTypeDef GDMA_InitStructTest;
	PGDMA_InitTypeDef   GDMA_InitStruct;

	DiagPrintf("\n%s: \n", __FUNCTION__);

	//1. Set source & destination buffer
	for (int i = 0; i < DATA_SIZE; i++) {
		TestBuf1[i] = i;
	}
	DCache_CleanInvalidate((void *) TestBuf1, DATA_SIZE);
	DiagPrintf("Src=0x%x ==> Dst=0x%x, Len=%d\r\n", TestBuf1 + DMA_SRC_OFFSET, TestBuf2 + DMA_DST_OFFSET, DATA_SIZE);
	memset(TestBuf2, 0xff, DATA_SIZE);
	DCache_CleanInvalidate((void *) TestBuf2, DATA_SIZE);

	//2. Configure GDMA parameters
	assert_param(GdmaType > SingleBlock && GdmaType < MAXGdmaTestMode);
	memset(&GDMA_InitStructTest, 0, sizeof(GDMA_InitTypeDef));
	GDMA_InitStruct = (PGDMA_InitTypeDef) &GDMA_InitStructTest;

	GDMA_StructInit(GDMA_InitStruct);
	GDMA_InitStruct->GDMA_Index = 0;
	GDMA_InitStruct->GDMA_ChNum = GDMA_CHANNEL_ID;

	GDMA_InitStruct->GDMA_DIR    = TTFCMemToMem;
	GDMA_InitStruct->GDMA_DstInc = IncType;
	GDMA_InitStruct->GDMA_SrcInc = IncType;

	GDMA_InitStruct->GDMA_SrcAddr = (u32)(&TestBuf1[0]);
	GDMA_InitStruct->GDMA_DstAddr = (u32)(&TestBuf2[0]);
	GDMA_InitStruct->GDMA_SrcMsize = MsizeOne;
	GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthFourBytes;
	GDMA_InitStruct->GDMA_DstMsize = MsizeOne;
	GDMA_InitStruct->GDMA_DstDataWidth = TrWidthFourBytes;
	GDMA_ChnlRegister(0, GDMA_InitStruct->GDMA_ChNum);

	GDMA_InitStruct->MuliBlockCunt = 0;
	GDMA_InitStruct->MaxMuliBlock = DATA_SIZE / BLOCK_SIZE;
	GDMA_InitStruct->GDMA_IsrType = (BlockType | TransferType | ErrType);
	Gdma_BlockLen = BLOCK_SIZE;
	if (GdmaType == ReloadDst) {
		DiagPrintf("ReloadDst Test\n");
		GDMA_InitStruct->GDMA_ReloadSrc = 0;
		GDMA_InitStruct->GDMA_ReloadDst = 1;
	} else if (GdmaType == ReloadSrc) {
		DiagPrintf("ReloadSar Test\n");
		GDMA_InitStruct->GDMA_ReloadSrc = 1;
		GDMA_InitStruct->GDMA_ReloadDst = 0;
	} else if (GdmaType == ReloadSrcDst) {
		DiagPrintf("ReloadSarDar Test\n");
		GDMA_InitStruct->GDMA_ReloadSrc = 1;
		GDMA_InitStruct->GDMA_ReloadDst = 1;
	}

	if (((BLOCK_SIZE & 0x03) != 0) || (((u32)(&TestBuf1[0]) & 0x03) != 0)) {
		/* move 1 byte each transfer */
		GDMA_InitStruct->GDMA_SrcMsize   = MsizeFour;
		GDMA_InitStruct->GDMA_SrcDataWidth = TrWidthOneByte;
	}
	GDMA_InitStruct->GDMA_BlockSize = BLOCK_SIZE  >> GDMA_InitStruct->GDMA_SrcDataWidth; //based on src width

	//3. Clear interrupt handler flag and register interrupt
	dma_done = 0;
	IrqNum = GDMA_GetIrqNum(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum);
	InterruptRegister((IRQ_FUN)gdma_multiblock_irq, IrqNum, (u32)GDMA_InitStruct, INT_PRI_MIDDLE);
	InterruptEn(IrqNum, INT_PRI_MIDDLE);

	//4. Intialize GDMA and enable GDMA
	GDMA_Init(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, GDMA_InitStruct);
	GDMA_Cmd(GDMA_InitStruct->GDMA_Index, GDMA_InitStruct->GDMA_ChNum, ENABLE);

	//5. Wait for the GDMA transfer to complete
	while (dma_done == 0);
	while (1);
}

void gdma_multiblock_transfer_test(void)
{
	if (xTaskCreate((TaskFunction_t)gdma_multiblock_transfer_task, "GDMA MULTI BLOCK TRANSFER DEMO", (2048 / 4), NULL, (tskIDLE_PRIORITY + 1), NULL) != pdPASS) {
		DiagPrintf("Cannot create dma_multiblock_transfer_task demo task\n\r");
	}
}

