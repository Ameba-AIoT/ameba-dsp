/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_SOC_H_
#define _AMEBA_SOC_H_

/* rom headers */
#include "ameba.h"

#define PLATFORM_FREERTOS

#ifdef PLATFORM_FREERTOS
#include "ameba_freertos_pmu.h"
#include "FreeRTOS.h"
#include "task.h"
#endif

#endif //_AMEBA_SOC_H_
