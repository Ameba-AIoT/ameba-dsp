/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef UTILS_IPC_RPC_UTILS_H
#define UTILS_IPC_RPC_UTILS_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"


#ifdef __cplusplus
extern "C" {
#endif

void RPC_Print(const char *tag, const char *fmt, ...)
__attribute__((__format__(printf, 2, 3)));

#define RPC_LOGD(...)  ((void)RPC_Print(LOG_TAG, __VA_ARGS__))
#define RPC_LOGE(fmt, ...)  printf(LOG_TAG fmt, ##__VA_ARGS__)

#define ENTER() \
	RPC_LOGD("%s Enter.\n", __func__);

#define RPC_SEM_WAIT_FOREVER 0xFFFFFFFF

typedef struct RPC_Mutex {
	SemaphoreHandle_t sema;
} __attribute__((aligned(4))) RPC_Mutex;

typedef struct RPC_Sem {
	SemaphoreHandle_t semaphore;
	int32_t value;  // semaphore count.
} __attribute__((aligned(4))) RPC_Sem;


void RPC_MutexLock(RPC_Mutex *mutex);

void RPC_MutexUnlock(RPC_Mutex *mutex);

void RPC_MutexInit(RPC_Mutex *mutex);

void RPC_MutexDestroy(RPC_Mutex *mutex);

void RPC_SemInit(RPC_Sem *sem, uint32_t value);

int32_t RPC_SemWait(RPC_Sem *sem, uint32_t ms);

void RPC_SemPost(RPC_Sem *sem);

void RPC_SemDestroy(RPC_Sem *sem);

void RPC_SetLog(int enable);

#ifdef __cplusplus
}
#endif

#endif // UTILS_IPC_RPC_UTILS_H