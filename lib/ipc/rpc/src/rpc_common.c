/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "RPCCommon"
#include "rpc_utils.h"

#include "xdr/xdr.h"
#include "xdr/xdr_prefix.h"

#include "rpc_struct.h"

#include "rpc_api.h"
#include "rpc_common.h"
#include "rpc_client.h"
#include "rpc_server.h"

#include "ameba_soc.h"

#define RPC_CHANNEL_AP2DSP (RPC_AP_DSP<<4) | (BLOCK_MODE | (RPC_DSP_AP << 1))
#define RPC_CHANNEL_NP2DSP (RPC_NP_DSP<<4) | (BLOCK_MODE | (RPC_DSP_NP << 1))


static RPCHwManager *g_rpc_manager = NULL;
static RPC_Mutex g_lock;
static bool g_inited = false;

/*
 * XDR type of struct RPC_STRUCT
 */
bool_t xdr_RPC_STRUCT(XDR *xdrs, RPC_STRUCT *objp)
{
	if (!xdr_u_long(xdrs, &objp->program_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->version_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->procedure_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->task_id)) {
		return FALSE;
	}
	if (!xdr_u_long(xdrs, &objp->parameter_size)) {
		return FALSE;
	}
	if (!xdr_u_int(xdrs, &objp->context)) {
		return FALSE;
	}

	return TRUE;
};

/*
 * assign user's setting value into the CLNT_STRUCT
 */
CLNT_STRUCT RPC_PrepareCLNT(int32_t opt, long program_id, long version_id)
{
	CLNT_STRUCT clnt;
	clnt.send_mode = opt;
	clnt.program_id = program_id;
	clnt.version_id = version_id;
	return clnt;
}

static void RPC_InitThread(void *param)
{
	RPC_LOGD("RPC_InitThread enter");
	(void)param;
	if (g_rpc_manager) {
		RPC_LOGE("RPC_Init already DONE.\n");
		vTaskDelete(NULL);
		return;
	}
	RPC_MutexInit(&g_lock);
	if (!g_rpc_manager) {
		g_rpc_manager = GetRPCManager();
	}
	if (g_rpc_manager) {
		RPC_LOGD("GetRPCManager Success.\n");
	} else {
		RPC_LOGE("GetRPCManager failed.\n");
	}

	// Register start
	// End
	g_inited = true;
	RPC_LOGD("RPC_InitThread finish");
	vTaskDelete(NULL);
}

/*
 * init RPC
 */
int32_t RPC_Init(void)
{
	if (xTaskCreate(RPC_InitThread, ((const char *)"RPC_InitThread"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		RPC_LOGE("\n\r%s xTaskCreate(RPC_InitThread) failed", __FUNCTION__);
	}
	return 0;
}

void RPC_DeInit(void)
{
	RPC_DeInitClient();
	RPC_DeInitServer();
	RPC_MutexLock(&g_lock);
	if (g_rpc_manager) {
		DestoryRPCHwManager(g_rpc_manager);
		g_rpc_manager = NULL;
	}
	RPC_MutexUnlock(&g_lock);
	RPC_MutexDestroy(&g_lock);
}

RPCHwManager *GetRPCManager(void)
{
	RPC_MutexLock(&g_lock);
	if (!g_rpc_manager) {
		g_rpc_manager = GetRPCHwManager();
	}
	RPC_MutexUnlock(&g_lock);
	return g_rpc_manager;
}

int32_t RPC_IsInited(void)
{
	RPC_LOGD("RPC_IsInited enter");
	int32_t ret = false;
	RPC_MutexLock(&g_lock);
	ret = g_inited ? 1 : 0;
	RPC_MutexUnlock(&g_lock);
	return ret;
}
