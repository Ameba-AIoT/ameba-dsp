/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef AMEBA_BASE_RPC_SRC_RPC_COMMON_H
#define AMEBA_BASE_RPC_SRC_RPC_COMMON_H

#include "rpc_struct.h"
#include "rpc_hw_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

bool_t xdr_RPC_STRUCT(XDR *xdrs, RPC_STRUCT *objp);
RPCHwManager *GetRPCManager(void);

#ifdef __cplusplus
}
#endif

#endif // AMEBA_BASE_RPC_SRC_RPC_COMMON_H
