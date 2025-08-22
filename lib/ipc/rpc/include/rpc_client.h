/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AMEBA_BASE_RPC_SRC_RPC_CLIENT_H
#define AMEBA_BASE_RPC_SRC_RPC_CLIENT_H

#include "rpc_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t WaitReply(void);

RPC_STRUCT RPC_PrepareCall(CLNT_STRUCT *clnt, int32_t result);

int32_t RPC_ClientCall(RPC_STRUCT *rpc, u_long procedure_id, int32_t opt, xdrproc_t xdr_args, caddr_t args_ptr, long args_size);

void RPC_DeInitClient(void);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_RPC_SRC_RPC_CLIENT_H
