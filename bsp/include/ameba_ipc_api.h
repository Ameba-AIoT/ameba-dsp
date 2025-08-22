/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBAD2_IPC_API_H
#define _AMEBAD2_IPC_API_H

typedef struct _IPC_MSG_STRUCT_ {
	u32 msg_type;
	u32 msg;
	u32 msg_len;
	u32 rsvd;
} IPC_MSG_STRUCT, *PIPC_MSG_STRUCT;

#ifdef CONFIG_LONGER_CMD
#define UART_LOG_CMD_BUFLEN     512
#else
//UART_LOG_CMD_BUFLEN: only 126 bytes could be used for keeping input
//                                     cmd, the last byte is for string end ('\0').
#define UART_LOG_CMD_BUFLEN     127
#endif

typedef struct {
#ifdef CONFIG_LONGER_CMD
	u16  BufCount;                           //record the input cmd char number.
#else
	u8  BufCount;                           //record the input cmd char number.
#endif
	u8  UARTLogBuf[UART_LOG_CMD_BUFLEN];   //record the input command.
} UART_LOG_BUF, *PUART_LOG_BUF;

void ipc_table_init(IPC_TypeDef *IPCx);
u32 ipc_send_message(u32 IPC_Dir, u8 IPC_ChNum, PIPC_MSG_STRUCT IPC_Msg);
PIPC_MSG_STRUCT ipc_get_message(u32 IPC_Dir, u8 IPC_ChNum);
void IPC_TXHandler(VOID *Data, u32 IrqStatus, u32 ChanNum);
extern IPC_IRQ_FUN IPC_IrqHandler[IPC_CHANNEL_NUM];

#define IPC_SEND_SUCCESS 		0
#define IPC_SEMA_TIMEOUT 		1
#define IPC_REQ_TIMEOUT 		2
#define IPC_SEND_TIMEOUT 		3
#define IPC_SEND_FAIL 		    4

#define IPC_SEMA_MAX_DELAY		0xFFFFFFFF
#define IPC_DSP_ID 				2


#endif
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
