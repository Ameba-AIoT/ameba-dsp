/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"
#include "FreeRTOS.h"
#include "semphr.h"


static u32 __kr4_ipc_memory_start__;

extern unsigned char _ipc_table_start[];
extern unsigned char _ipc_table_end[];

SemaphoreHandle_t ipc_Semaphore[IPC_TX_CHANNEL_NUM];

/**
  * @brief  init ipc interrupt handler table.
  * @param  where IPCx can be IPCKR4_DEV for KR4, IPCKM4_DEV for CM4, IPCDSP_DEV for DSP.
  * @retval   None
  */

void ipc_table_init(IPC_TypeDef *IPCx)
{
	u32 i;
	u32 IPC_Dir;
	u32 IPC_ChNum;
	u32 IPC_GroupShift;
	u32 IPC_ChShift;
	u32 IPC_IMR;
	u32 IPC_IntMode;

	__kr4_ipc_memory_start__ = RRAM_DEV->IPC_Share_Mem;

#if defined ( __ICCARM__ )
	__ipc_table_start__ = (u8 *)__section_begin(".ipc.table.data");
	__ipc_table_end__ = (u8 *)__section_end(".ipc.table.data");
#endif

	if ((&_ipc_table_start) == (&_ipc_table_end)) {
		DelayMs(50);
		printf("========================================================================= \n"
			   "                         IPC Table Empty Warning                          \n"
			   "========================================================================= \n"
			   "  _ipc_table_start:%p, _ipc_table_end:%p    \n"
			   "  If you need IPC function, please add KEEP in LSP path: elf32xtensa.x    \n"
			   "  Reference: Application Note \"Reduce DSP Image Size\" section           \n"
			   "========================================================================= \n"
			   , &_ipc_table_start, &_ipc_table_end
			  );
	}

	IPC_INIT_TABLE *ipc_init_table = (IPC_INIT_TABLE *)(&_ipc_table_start);
	u32 ipc_num = (((u32)(&_ipc_table_end) - (u32)(&_ipc_table_start)) / sizeof(IPC_INIT_TABLE));


	for (i = 0; i < ipc_num; i++) {
		IPC_Dir = ipc_init_table[i].IPC_Direction;
		IPC_ChNum = ipc_init_table[i].IPC_Channel;

		/* check ipc entry has register on a correct cpu */
		if (IS_SEND_TO_KM4(IPC_Dir)) {

			IPC_IntMode = (IPCx == IPCKM4_DEV) ? IPC_RX_FULL : IPC_TX_EMPTY;

		} else if (IS_SEND_TO_KR4(IPC_Dir)) {
			IPC_IntMode = (IPCx == IPCKR4_DEV) ? IPC_RX_FULL : IPC_TX_EMPTY;

		} else if (IS_SEND_TO_DSP(IPC_Dir)) {

			IPC_IntMode = (IPCx == IPCDSP_DEV) ? IPC_RX_FULL : IPC_TX_EMPTY;
		} else {
			continue;

		}

		/* set ipc group shift according to intr mode */
		if (IPC_IntMode == IPC_RX_FULL) {
			if ((IPC_Dir == IPC_KM4_TO_KR4) || (IPC_Dir == IPC_KR4_TO_KM4) || (IPC_Dir == IPC_KR4_TO_DSP)) {
				IPC_GroupShift = 24;
			} else {
				IPC_GroupShift = 16;
			}
		} else {
			if ((IPC_Dir == IPC_KM4_TO_KR4) || (IPC_Dir == IPC_KR4_TO_KM4) || (IPC_Dir == IPC_DSP_TO_KR4)) {
				IPC_GroupShift = 8;
			} else {
				IPC_GroupShift = 0;
			}
		}

		IPC_ChShift = IPC_ChNum + IPC_GroupShift;

		/* check if channel conflict */
		IPC_IMR = IPC_IERGet(IPCx);
		if (IPC_IMR & BIT(IPC_ChShift)) {
			continue;
			/* overwrite the old call back func for cpu reset happens */
			//DBG_PRINTF(MODULE_IPC, LEVEL_INFO, "Channel Conflict for CPU %d Channel %d ! Ignore If CPU Has Reset\r\n", SYS_CPUID(), IPC_ChShift);
		}

		if (IPC_IntMode == IPC_RX_FULL) {
			/* enable rx intr and register call back func*/
			IPC_INTUserHandler(IPCx, (u8)IPC_ChShift, (void *)ipc_init_table[i].Rxfunc, ipc_init_table[i].RxIrqData);
		} else {
			/* enable tx intr and register call back func*/
			IPC_INTUserHandler(IPCx, (u8)IPC_ChShift, (void *)ipc_init_table[i].Txfunc, ipc_init_table[i].TxIrqData);
		}
	}
}


/**
  * @brief  The common IPC Tx interrupt handler
  * @param  Data: the data pointer to IPCx
  * @param  IrqStatus: Value of IPC_ISR
  * @param  ChanNum: ChanNum
  */
void IPC_TXHandler(VOID *Data, u32 IrqStatus, u32 ChanNum)
{

	UNUSED(Data);
	UNUSED(IrqStatus);

	IPC_TypeDef *IPCx = IPC_GetDevById(IPC_DSP_ID);

	portBASE_TYPE taskWoken = pdFALSE;

	IPC_INTConfig(IPCx, (u8)ChanNum, DISABLE);

	if (ipc_Semaphore[ChanNum] != NULL) {
		xSemaphoreGiveFromISR(ipc_Semaphore[ChanNum], &taskWoken);
//		portEND_SWITCHING_ISR(taskWoken);
	}

}

/**
  * @brief  Processing functions when the IPC channel is occupied
  * @param  IPCx: where IPCx can be IPCKM0_DEV for KM0, IPCKM4_DEV for CM4.
  * @param  IPC_ChNum: IPC_ChNum
  * @retval IPC_REQ_TIMEOUT or IPC_SEMA_TIMEOUT or SUCCESS
  */
u32 IPC_wait_idle(IPC_TypeDef *IPCx, u32 IPC_ChNum)
{
	u32 timeout;

	timeout = 10000000;

	if (CPU_InInterrupt() || (IPC_IrqHandler[IPC_ChNum] == NULL)) {
		while (IPCx->IPC_TX_DATA & (BIT(IPC_ChNum))) {
			timeout--;
			if (timeout == 0) {
				printf(" IPC Request Timeout\r\n");
				return IPC_REQ_TIMEOUT;
			}
		}
	} else {
		if (ipc_Semaphore[IPC_ChNum] == NULL) {
			vSemaphoreCreateBinary(ipc_Semaphore[IPC_ChNum]);
			xSemaphoreTake(ipc_Semaphore[IPC_ChNum], 1 / portTICK_RATE_MS);
		}

		IPC_INTConfig(IPCx, (u8)IPC_ChNum, ENABLE);

		if (xSemaphoreTake(ipc_Semaphore[IPC_ChNum], IPC_SEMA_MAX_DELAY) != pdTRUE) {
			printf(" IPC Get Semaphore Timeout\r\n");
			IPC_INTConfig(IPCx, (u8)IPC_ChNum, DISABLE);
			return IPC_SEMA_TIMEOUT;
		}
	}
	return 0;
}


/**
  * @brief  exchange messages between KM0 and KM4.
  * @param  IPC_Dir: Specifies core to core direction
  *          This parameter can be one of the following values:
  *		 		@arg IPC_KR4_TO_KM4: KR4 send request to KM4
  *		 		@arg IPC_KR4_TO_DSP: KR4 send request to DSP
  *		 		@arg IPC_KM4_TO_KR4: KM4 send request to KR4
  *		 		@arg IPC_KM4_TO_DSP: KM4 send request to DSP
  *		 		@arg IPC_DSP_TO_KR4: DSP send request to KR4
  *		 		@arg IPC_DSP_TO_KM4: DSP send request to KM4
  * @param  IPC_ChNum: the IPC channel number.
  * @param  Message: pointer to the message to be exchanged,and should not stored in stack.
  * @retval   None
  */
u32 ipc_send_message(u32 IPC_Dir, u8 IPC_ChNum, PIPC_MSG_STRUCT IPC_Msg)
{
	u32 ipc_shift = 0;
	/* Check the parameters */
	assert_param(IS_IPC_DIR_MODE(IPC_Dir));

	PIPC_MSG_STRUCT IPC_MSG = (PIPC_MSG_STRUCT)__kr4_ipc_memory_start__;
	u32 msg_idx = IPC_TX_CHANNEL_NUM * IPC_TX_CHANNEL_SWITCH(IPC_Dir) + IPC_TX0_CHANNEL_NUM * IPC_TX0_CHANNEL_SWITCH(IPC_Dir) + IPC_ChNum;
	IPC_TypeDef *IPCx = NULL;

	switch (IPC_Dir) {
	case IPC_KR4_TO_KM4:
		ipc_shift = 8;
		IPCx = IPCKR4_DEV;
		break;
	case IPC_KR4_TO_DSP:
		IPCx = IPCKR4_DEV;
		break;

	case IPC_KM4_TO_KR4:
		ipc_shift = 8;
		IPCx = IPCKM4_DEV;
		break;
	case IPC_KM4_TO_DSP:
		IPCx = IPCKM4_DEV;
		break;

	case IPC_DSP_TO_KR4:
		ipc_shift = 8;
		IPCx = IPCDSP_DEV;
		break;
	case IPC_DSP_TO_KM4:
		IPCx = IPCDSP_DEV;
		break;

	default:
		printf("DSP IPC direction error:%d, %s \n", __LINE__, __FILE__);
		while (1);
		break;
	}

	if (IPCx->IPC_TX_DATA & (BIT(IPC_ChNum + ipc_shift))) {
		if (IPC_wait_idle(IPCx, IPC_ChNum + ipc_shift)) {
			return IPC_SEND_TIMEOUT;
		}
	}

	if (IPC_Msg) {
		memcpy(&IPC_MSG[msg_idx], IPC_Msg, sizeof(IPC_MSG_STRUCT));
		DCache_Clean((void *)&IPC_MSG[msg_idx], sizeof(IPC_MSG_STRUCT));
	}
	IPCx->IPC_TX_DATA = (BIT(IPC_ChNum + ipc_shift));
	return IPC_SEND_SUCCESS;
}

/**
  * @brief  get ipc message.
  * @param  IPC_Dir: Specifies core to core direction
  *          This parameter can be one of the following values:
  *		 		@arg IPC_KR4_TO_KM4: KR4 send request to KM4
  *		 		@arg IPC_KR4_TO_DSP: KR4 send request to DSP
  *		 		@arg IPC_KM4_TO_KR4: KM4 send request to KR4
  *		 		@arg IPC_KM4_TO_DSP: KM4 send request to DSP
  *		 		@arg IPC_DSP_TO_KR4: DSP send request to KR4
  *		 		@arg IPC_DSP_TO_KM4: DSP send request to KM4
  * @param  IPC_ChNum: the IPC channel number.
  * @retval  : pointer to the message to be exchanged.
  * @note for data massage, corresponding data cache should be invalidate before access.
  */
PIPC_MSG_STRUCT ipc_get_message(u32 IPC_Dir, u8 IPC_ChNum)
{
	PIPC_MSG_STRUCT IPC_MSG = NULL;
	u32 msg_idx = IPC_TX_CHANNEL_NUM * IPC_TX_CHANNEL_SWITCH(IPC_Dir) + IPC_TX0_CHANNEL_NUM * IPC_TX0_CHANNEL_SWITCH(IPC_Dir) + IPC_ChNum;

	IPC_MSG = (PIPC_MSG_STRUCT)__kr4_ipc_memory_start__;
	DCache_Invalidate((void *)&IPC_MSG[msg_idx], sizeof(IPC_MSG_STRUCT));

	return &IPC_MSG[msg_idx];
}
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
