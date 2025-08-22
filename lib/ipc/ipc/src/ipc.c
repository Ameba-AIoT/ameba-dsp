/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "ipc.h"

#include "ameba.h"
#include "sysreg_lsys.h"
#include "FreeRTOS.h"
#include "semphr.h"


#define IPC_RX_TRAN_0	IPC_M2D_IMQ_TRX_TRAN
#define IPC_RX_TRAN_1	IPC_R2D_IMQ_TRX_TRAN
#define IPC_TX_TRAN_0	IPC_D2R_IMQ_TRX_TRAN
#define IPC_TX_TRAN_1	IPC_D2M_IMQ_TRX_TRAN
#define IPC_DIR_MSG_RX_0 IPC_KM4_TO_DSP
#define IPC_DIR_MSG_RX_1 IPC_KR4_TO_DSP

#define BYTE_ALIGNMENT		128

#define IPC_TABLE_DATA_SECTION __attribute__ ((section(".ipc_table")))

typedef struct {
	uint32_t addr;
	uint32_t len;
	uint32_t dir;
	uint32_t rsvd0[29];
	uint32_t wptr;
	uint32_t rsvd1[31];
	uint32_t rptr;
	uint32_t rsvd2[31];
} MessageInfo;


static void *g_wake_sema[IPC_ID_NUM];
static uint32_t g_ipc_buffer = 0;
static const uint32_t g_ipc_irq_data0 = 0;
static const uint32_t g_ipc_irq_data1 = 1;

int32_t IPC_Message_Queue_Init(void)
{
	RRAM_TypeDef *rram = IPC_RRAM;
	IPC_LOGI("Ipc_Message_Init IMQ2_INIT_DONE=%d\n", rram->IMQ2_INIT_DONE);
	if (g_ipc_buffer) {
		IPC_LOGE("IMQ2_INIT_DONE aleady\n");
		return IPC_ERROR;
	}

	while (rram->IMQ2_INIT_DONE == 0) {
		vTaskDelay(2);
	}
	g_ipc_buffer = rram->IMQ2_HEAP_ADDR;
	for (uint8_t i = 0; i < IPC_ID_NUM; i++) {
		g_wake_sema[i] = xSemaphoreCreateBinary();
	}
	IPC_LOGI("Ipc_Message_Init End\n");
	return 0;
}

int32_t IPC_Message_Queue_Send(uint32_t id, uint8_t *buf, uint32_t size, int32_t timeout)
{
	MessageInfo *info;
	uint32_t segment_len = 0;
	uint32_t remain_bytes = 0;
	uint32_t end_addr;

	if (id > IPC_ID_NUM) {
		IPC_LOGE("id: %d IPC_INVALID_CH\n", (int)id);
		return IPC_INVALID_CH;
	}

	if (!size || !buf) {
		IPC_LOGE("id: %d, invalid size or buf\n", (int)id);
		return IPC_INVALID_SIZE;
	}

	info = (MessageInfo *)(g_ipc_buffer + id * sizeof(MessageInfo));
	DCache_Invalidate((void *)info, sizeof(MessageInfo));

	do {
		uint32_t rptr;
		rptr = info->rptr;
		if (rptr > info->wptr) {
			remain_bytes = rptr - info->wptr - 1;
		} else {
			remain_bytes = rptr + info->len - info->wptr - 1;
		}
		if (remain_bytes > size) {
			break;
		}

		if (timeout == 0) {
			return IPC_TX_TIMEOUT;
		}
		if (timeout != WAIT_FOREVER) {
			timeout--;
		}
		vTaskDelay(1);
		DCache_Invalidate((void *)&info->rptr, BYTE_ALIGNMENT);
	} while (timeout > 0 || timeout == WAIT_FOREVER);

	end_addr = info->addr + info->len;
	if (info->wptr + size > end_addr) {
		segment_len = end_addr - info->wptr;
		memcpy((void *)info->wptr, (void *)buf, segment_len);
		memcpy((void *)info->addr, (void *)(buf + segment_len), size - segment_len);
		DCache_Clean((void *)info->wptr, segment_len);
		DCache_Clean((void *)info->addr, size - segment_len);
		info->wptr = info->addr + (size - segment_len);
	} else {
		memcpy((void *)info->wptr, (void *)buf, size);
		DCache_Clean((void *)info->wptr, size);
		info->wptr += size;
	}
	DCache_Clean((void *) & (info->wptr), BYTE_ALIGNMENT);
	uint8_t channel = 0;
	if (info->dir & 0x00000001) {
		channel = IPC_TX_TRAN_1;
	} else {
		channel = IPC_TX_TRAN_0;
	}
	IPC_MSG_STRUCT ipc_message = { 0 };
	ipc_message.msg = id;
	ipc_message.msg_type = IPC_USER_POINT;
	ipc_send_message(info->dir, channel, &ipc_message);
	IPC_LOGV("ipc_send_message: %d, channel=%d\n", (int)info->dir, channel);
	return IPC_SUCCESS;
}

int32_t IPC_Message_Queue_Recv(uint32_t id, uint8_t *buf, uint32_t *size)
{
	MessageInfo *info;
	uint32_t end_addr;
	uint32_t segment_len = 0;
	uint32_t readable_size = 0;
	uint32_t wptr;

	if (!buf || *size == 0) {
		IPC_LOGE("id: %d, invalid buf or size : %d\n", (int)id, (int)*size);
		return IPC_INVALID_SIZE;
	}

	if (id >= IPC_ID_NUM) {
		IPC_LOGE("id: %dis available\n", (int)id);
		return IPC_INVALID_CH;
	}

	info = (MessageInfo *)(g_ipc_buffer + id * sizeof(MessageInfo));
	DCache_Invalidate((void *)info, sizeof(MessageInfo));

	do {
		wptr = info->wptr;
		if (info->rptr > wptr) {
			readable_size = wptr + info->len - info->rptr;
		} else {
			readable_size = wptr - info->rptr;
		}

		if (readable_size < *size) {
			xSemaphoreTake(g_wake_sema[id], 0xFFFFFFFF);
			DCache_Invalidate((void *)info, sizeof(MessageInfo));
		}
	} while (readable_size < *size);

	end_addr = info->addr + info->len;
	if (info->rptr + *size > end_addr) {
		segment_len = end_addr - info->rptr;
		DCache_Invalidate((void *)info->rptr, segment_len);
		DCache_Invalidate((void *)info->addr, *size - segment_len);
		memcpy((void *)buf, (void *)info->rptr, segment_len);
		memcpy((void *)(buf + segment_len), (void *)info->addr, *size - segment_len);
		info->rptr = info->addr + (*size - segment_len);
	} else {
		DCache_Invalidate((void *)info->rptr, *size);
		memcpy((void *)buf, (void *)info->rptr, *size);
		info->rptr += *size;
	}
	DCache_Clean((void *)&info->rptr, BYTE_ALIGNMENT);
	return IPC_SUCCESS;
}

static void IPC_Message_Handler(VOID *data, uint32_t irq_status, uint32_t chan_num)
{
	(void)irq_status;
	(void)chan_num;
	uint32_t irq_data = *(uint32_t *)data;

	PIPC_MSG_STRUCT p_recv_msg;
	switch (irq_data) {
	case g_ipc_irq_data0:
		p_recv_msg = ipc_get_message(IPC_DIR_MSG_RX_0, IPC_RX_TRAN_0);
		break;
	case g_ipc_irq_data1:
		p_recv_msg = ipc_get_message(IPC_DIR_MSG_RX_1, IPC_RX_TRAN_1);
		break;
	default:
		IPC_LOGE("DSP IPC_Message_Handler error:%d, %s \n", __LINE__, __FILE__);
		while (1);
		break;
	}

	DCache_Invalidate((void *)p_recv_msg, sizeof(IPC_MSG_STRUCT));
	IPC_LOGV("%s, id: %d\n", __func__, p_recv_msg->msg);
	if (g_wake_sema[p_recv_msg->msg]) {
		portBASE_TYPE task_woken = pdFALSE;
		xSemaphoreGiveFromISR(g_wake_sema[p_recv_msg->msg], &task_woken);
		portYIELD_FROM_ISR(task_woken);
	}
}

#if defined(CONFIG_RPC_EN) && CONFIG_RPC_EN
__attribute__((section(".ipc_table")))
const IPC_INIT_TABLE rpc_message_handler0 = {
	.USER_MSG_TYPE = IPC_USER_POINT,
	.Rxfunc = IPC_Message_Handler,
	.RxIrqData = (VOID *) &g_ipc_irq_data0,
	.Txfunc = (VOID *) NULL,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_DIR_MSG_RX_0,
	.IPC_Channel = IPC_RX_TRAN_0
};

__attribute__((section(".ipc_table")))
const IPC_INIT_TABLE rpc_message_handler1 = {
	.USER_MSG_TYPE = IPC_USER_POINT,
	.Rxfunc = IPC_Message_Handler,
	.RxIrqData = (VOID *) &g_ipc_irq_data1,
	.Txfunc = (VOID *) NULL,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_DIR_MSG_RX_1,
	.IPC_Channel = IPC_RX_TRAN_1
};
#endif
