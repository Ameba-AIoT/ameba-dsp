
/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BASIC_TYPES_H__
#define __BASIC_TYPES_H__

/**
  * @brief get chip UUID & LOT_NUM
  * UUID is 64 bits, so it requires 8 bytes of space.
  * DSP UUID API same with MCU.
  */
extern void EFUSE_GetUUID(u32 *UUID);

#endif
