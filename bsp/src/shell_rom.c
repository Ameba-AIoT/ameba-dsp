/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"
#include <stdarg.h>
//#include "strproc.h"

#define MAX_ARGV                16
//=================================================
u8						*shell_argv_array[MAX_ARGV];
//======================================================
//<Function>:  shell_array_init
//<Usage   >:  This function is to initialize an array with a given value.
//<Argus    >:  pArrayToInit --> pointer to an array
//                   ArrayLen      --> number of array elements
//                   InitValue     --> initial value for the array
//<Return   >:  VOID
//<Notes    >:  NA
//======================================================
void shell_array_init(u8  *pArrayToInit,  u8  ArrayLen,  u8  InitValue)
{
	u8  ItemCount = 0;

	for (ItemCount = 0; ItemCount < ArrayLen; ItemCount++) {
		*(pArrayToInit + ItemCount) = InitValue;
	}
}


//======================================================
//<Function>:  shell_get_argc
//<Usage   >:  This function is to calcute how many parameters in the cmd string
//<Argus    >:  string      --> pointer to an input cmd string
//<Return   >:  number of parameters in the string.(****except the cmd itself****)
//<Notes    >:  NA
//======================================================
u8 shell_get_argc(const   u8  *string)
{
	u8 argc;
	u8  *pStr;

	argc = 0;
	pStr = (u8 *)string;

	while (*pStr) {
		if ((*pStr != ' ') && (*pStr)) {
			argc++;
			while ((*pStr != ' ') && (*pStr)) {
				pStr++;
			}

			continue;
		}

		pStr++;
	}

	if (argc >= MAX_ARGV) {
		argc = MAX_ARGV - 1;
	}

	return argc;
}

//======================================================
//<Function>:  shell_get_argv
//<Usage   >:  This function is to transfer a cmd string to a char array.
//<Argus    >:  string      --> pointer to an input cmd string
//<Return   >:  pointer to a char array
//<Notes    >:  NA
//======================================================
u8 **shell_get_argv(const   u8  *string)
{
	u8  ArgvCount = 0;
	u8  *pStr;

	shell_array_init((u8 *)shell_argv_array, MAX_ARGV * sizeof(char *), 0);
	pStr = (u8 *)string;

	while (*pStr) {
		shell_argv_array[ArgvCount] = pStr;
		while ((*pStr != ' ') && (*pStr)) {
			pStr++;
		}

		*(pStr++) = '\0';

		while ((*pStr == ' ') && (*pStr)) {
			pStr++;
		}

		ArgvCount++;

		if (ArgvCount == MAX_ARGV) {
			break;
		}
	}

	return (u8 **)&shell_argv_array;
}






