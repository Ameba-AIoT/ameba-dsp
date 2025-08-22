/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IPC_MESSAGE_H
#define IPC_MESSAGE_H
#include <stdint.h>

//#define DEBUG_ENABLE

#define TAG "[IPC_MSG]"
#define TAG_ERR "[IPC_MSG][ERR]"

#ifdef DEBUG_ENABLE
#define IPC_LOGV(fmt, ...)  DiagPrintf(TAG fmt, ##__VA_ARGS__)
#else
#define IPC_LOGV(fmt, ...)  do { } while(0)
#endif
#define IPC_LOGI(fmt, ...)  DiagPrintf(TAG fmt, ##__VA_ARGS__)
#define IPC_LOGE(fmt, ...)  DiagPrintf(TAG_ERR fmt, ##__VA_ARGS__)

#define WAIT_FOREVER	-1
#define IPC_RRAM RRAM_DEV
#define IPC_ID_NUM			20

typedef enum {
	IPC_SUCCESS         =  0,
	IPC_ERROR           = -1,
	IPC_INVALID_CH      = -2,
	IPC_INVALID_SIZE    = -3,
	IPC_NO_MEMORY       = -4,
	IPC_NOT_INIT        = -5,
	IPC_TX_TIMEOUT      = -6,
	IPC_RX_TIMEOUT      = -7,
	IPC_TX_ERROR        = -8,
	IPC_RX_ERROR        = -9,
} ipc_error_t;


int32_t IPC_Message_Queue_Init(void);
int32_t IPC_Message_Queue_Send(uint32_t id, uint8_t *buf, uint32_t size, int32_t timeout);
int32_t IPC_Message_Queue_Recv(uint32_t id, uint8_t *buf, uint32_t *size);

#endif //IPC_MESSAGE_H
