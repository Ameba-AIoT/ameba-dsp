/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

u8 Boot_LoadDTCM()
{
	IMAGE_HEADER ImgHdr;
	RRAM_TypeDef *rram = RRAM_DEV;
	uint32_t DTCM0_ADDR = rram->DSPIMG_DTCM0_ADDR;
	uint32_t DTCM1_ADDR = rram->DSPIMG_DTCM1_ADDR;
	uint32_t DstAddr = 0, StartAddr = 0, Len = 0;
	uint8_t dtcm_seg_count = 2;

	for(int i = 0; i < dtcm_seg_count; i++){
		if(i == 0){
			StartAddr = DTCM0_ADDR;
		}else if(i == 1){
			StartAddr = DTCM1_ADDR;
		}

		memcpy((void*)(&ImgHdr), (void*)(StartAddr), IMAGE_HEADER_LEN);
		if ((ImgHdr.signature[0] != 0x35393138) || (ImgHdr.signature[1] != 0x31313738)) {
			return 0;
		}
		DstAddr = ImgHdr.image_addr;
		Len = ImgHdr.image_size + IMAGE_HEADER_LEN;

		memcpy((void*)DstAddr, (void*)(StartAddr + IMAGE_HEADER_LEN), Len - IMAGE_HEADER_LEN);
	}
	return 0;
}
