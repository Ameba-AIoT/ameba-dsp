/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _BOOT_LOAD_DTCM_H_
#define _BOOT_LOAD_DTCM_H_

#define MANIFEST_LEN		0x1000
#define FLASH_BIN_ADDR		0x08400000
#define IMAGE_HEADER_LEN		0x20
#define TOTAL_SEGMENT		5
#define DTCM0_INDEX			3
#define DTCM1_INDEX			4

typedef struct {
	u32 signature[2];
	u32 image_size;
	u32 image_addr;

	/* reserved for extention */
	u32 sb_header;
	u32 reserved[3];
} IMAGE_HEADER;

u8 Boot_LoadDTCM();

#endif
