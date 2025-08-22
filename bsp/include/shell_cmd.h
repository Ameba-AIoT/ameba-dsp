/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SHELL_CMD_H_
#define _SHELL_CMD_H_

extern u8 **shell_get_argv(const   u8  *string);
extern u8 shell_get_argc(const   u8  *string);

void shell_cmd_init();

typedef struct _COMMAND_TABLE_ {
	const   u8 *cmd;
	u16     ArgvCnt;
	u32(*func)(u16 argc, u8 *argv[]);
	const   u8 *msg;
} COMMAND_TABLE, *PCOMMAND_TABLE;

#endif
