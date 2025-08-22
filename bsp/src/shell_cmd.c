/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define CMD_QUEUE_LEN 10
#define CMD_QUEUE_ELE 4
static QueueHandle_t xQueue_cmd = NULL;

extern unsigned char _command_start[];
extern unsigned char _command_end[];

void uart_log(void *Data, u32 data0, u32 data2)
{
	PIPC_MSG_STRUCT pipc_msg = ipc_get_message(IPC_KM4_TO_DSP, IPC_M2D_LOGUART_RX_SWITCH);
	u32 msg_addr = pipc_msg -> msg;
	int res = xQueueSendFromISR(xQueue_cmd, &(msg_addr), NULL);
}


static void shell_cmd_task()
{
	PCOMMAND_TABLE cmd_start = (PCOMMAND_TABLE)(&_command_start);
	PCOMMAND_TABLE cmd_end = (PCOMMAND_TABLE)(&_command_end);

	if ((&_command_start) == (&_command_end)) {
		DelayMs(50);
		printf("========================================================================= \n"
			   "                      Shell CMD Table Empty Warning                       \n"
			   "========================================================================= \n"
			   "  _command_start:%p, _command_end:%p    \n"
			   "  If you need CMD function, please add KEEP in LSP path: elf32xtensa.x    \n"
			   "  Reference: Application Note \"Reduce DSP Image Size\" section           \n"
			   "========================================================================= \n"
			   , &_command_start, &_command_end
			  );
	}

	BaseType_t xResult;
	while (1) {
		UART_LOG_BUF *buf;
		xResult = xQueueReceive(xQueue_cmd, &buf, portMAX_DELAY);

		if (xResult == pdPASS) {
			u8 argc = shell_get_argc(buf->UARTLogBuf);
			u8 **argv = shell_get_argv(buf->UARTLogBuf);

			PCOMMAND_TABLE table_entry = cmd_start;

			while (table_entry < cmd_end) {
				int res = strcmp((const char *)argv[0], (const char *)table_entry->cmd);
				if (res == 0) {
					table_entry->func((argc - 1), (argv + 1));

					break;
				} else {
					table_entry++;
				}
			}
		}
	}
}

__attribute__((section(".ipc_table")))
const IPC_INIT_TABLE ipc_shell_table = {
	.USER_MSG_TYPE = IPC_USER_DATA,
	.Rxfunc = uart_log,
	.RxIrqData = (VOID *) NULL,
	.Txfunc = (VOID *) NULL,
	.TxIrqData = (VOID *) NULL,
	.IPC_Direction = IPC_KM4_TO_DSP,
	.IPC_Channel = IPC_M2D_LOGUART_RX_SWITCH
};

void shell_cmd_init()
{
	ipc_table_init(IPCDSP_DEV);
	InterruptRegister((xt_handler)IPC_INTHandler, IPC_DSP_IRQ_DSP, IPCDSP_DEV, NULL);
	InterruptEn(IPC_DSP_IRQ_DSP, NULL);
	xQueue_cmd = xQueueCreate(CMD_QUEUE_LEN, CMD_QUEUE_ELE);
	xTaskCreate(shell_cmd_task, "shell_cmd_task", 1000, NULL, configMAX_PRIORITIES - 2, NULL);
}



