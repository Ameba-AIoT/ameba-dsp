/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AMEBA_RPC_FRAMEWORKS_INTERFACES_RPC_API_H
#define AMEBA_RPC_FRAMEWORKS_INTERFACES_RPC_API_H

#include "rpc_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t RPC_Init(void);

void RPC_DeInit(void);

CLNT_STRUCT RPC_PrepareCLNT(int32_t opt, long program_id, long version_id);

REG_STRUCT *ReplyHandler_register(struct REG_STRUCT *reg);

int32_t RPC_InitProxy(struct REG_STRUCT *reg, int32_t channel_id, RPC_INIT_STRUCT *init_param);

int32_t RPC_Unregister(struct REG_STRUCT *reg);

int32_t RPC_IsInited(void);

#ifdef __cplusplus
}
#endif
#endif //AMEBA_RPC_FRAMEWORKS_INTERFACES_RPC_API_H