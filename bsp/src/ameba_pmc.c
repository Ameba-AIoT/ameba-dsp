/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"
#include "ameba_freertos_pmu.h"

extern SLEEP_ParamDef sleep_param;

/**
  * @brief  set dsp wake up event mask0.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetDSPWakeEvent_MSK0(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK0_DSP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK0_DSP, WakeEvent);
}


/**
  * @brief  set dsp wake up event mask1.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetDSPWakeEvent_MSK1(u32 Option, u32 NewStatus)
{
	u32 WakeEvent = 0;

	/* Set Event */
	WakeEvent = HAL_READ32(PMC_BASE, WAK_MASK1_DSP);
	if (NewStatus == ENABLE) {
		WakeEvent |= Option;
	} else {
		WakeEvent &= ~Option;
	}
	HAL_WRITE32(PMC_BASE, WAK_MASK1_DSP, WakeEvent);
}


/**
  * @brief  set dsp wake up event.
  * @param  Option:
  *   This parameter can be any combination of the following values:
  *		 @arg WAKE_SRC_XXX
  * @param  NewStatus: TRUE/FALSE.
  * @retval None
  */
void SOCPS_SetDSPWakeEvent(u32 Option, u32 Group, u32 NewStatus)
{
	if (Group) {
		SOCPS_SetDSPWakeEvent_MSK1(Option, NewStatus);
	} else {
		SOCPS_SetDSPWakeEvent_MSK0(Option, NewStatus);
	}
}

void SOCPS_SleepCG(void)
{
	u32 KR4_is_NP = 0;

	KR4_is_NP = LSYS_GET_KR4_IS_NP(HAL_READ32(SYSTEM_CTRL_BASE, REG_LSYS_SYSTEM_CFG1));

	for (uint32_t x = 0; x < XCHAL_NUM_INTERRUPTS; x++) {
		if (xt_interrupt_enabled(x) & xt_interrupt_pending(x)) {
			DBG_8195A("int %d triggered\n", x);
			return;
		}
	}

	if (tickless_debug) {
		DBG_8195A("DSP CG\n");
	}

	sleep_param.sleep_type = SLEEP_CG;
	if (KR4_is_NP) {
		ipc_send_message(IPC_DSP_TO_KR4, IPC_D2R_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);
	} else {
		ipc_send_message(IPC_DSP_TO_KM4, IPC_D2M_TICKLESS_INDICATION, (PIPC_MSG_STRUCT)&sleep_param);
	}

	XT_ISYNC();
	XT_WAITI(0);

	if (tickless_debug) {
		DBG_8195A("DSP CGW\n");
	}
}


/**
  * @brief  KR4 wake KM4 by IPC
  * @param  None
  * @note  ipc_msg_temp.msg represents who wakes up KM4
  		* ipc_msg_temp.msg = 0: FW wakeup KM4
  * @retval None
  */
void SOCPS_KR4WDSP_ipc_int(void *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_KR4_TO_DSP, IPC_R2D_WAKE_DSP);

	u32 type = ipc_msg_temp->msg;

	if (tickless_debug) {
		DBG_8195A("type: %d\n", type);
	}
}

void SOCPS_KM4WDSP_ipc_int(void *Data, u32 IrqStatus, u32 ChanNum)
{
	/* To avoid gcc warnings */
	UNUSED(Data);
	UNUSED(IrqStatus);
	UNUSED(ChanNum);

	PIPC_MSG_STRUCT	ipc_msg_temp = (PIPC_MSG_STRUCT)ipc_get_message(IPC_KM4_TO_DSP, IPC_M2D_WAKE_DSP);

	u32 type = ipc_msg_temp->msg;

	if (tickless_debug) {
		DBG_8195A("type: %d\n", type);
	}
}

__attribute__((section(".ipc_table")))
const IPC_INIT_TABLE ipc_KR4WDSP_table = {
	.USER_MSG_TYPE = IPC_USER_DATA,
	.Rxfunc = SOCPS_KR4WDSP_ipc_int,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = (VOID *) NULL,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_KR4_TO_DSP,
	.IPC_Channel = IPC_R2D_WAKE_DSP
};

__attribute__((section(".ipc_table")))
const IPC_INIT_TABLE ipc_KM4WDSP_table = {
	.USER_MSG_TYPE = IPC_USER_DATA,
	.Rxfunc = SOCPS_KM4WDSP_ipc_int,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = (VOID *) NULL,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_KM4_TO_DSP,
	.IPC_Channel = IPC_M2D_WAKE_DSP
};

