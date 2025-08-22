/*
 * asm-offsets.h
 *
 *  Created on: 2022Äê5ÔÂ6ÈÕ
 *      Author: yan_jia
 */

#ifndef _ASM_OFFSETS_H
#define _ASM_OFFSETS_H

#if 1
#define TCB_TOP_OF_STACK_OFF 0 /* offsetof(TCB_t, pxTopOfStack) */
#define TCB_END_OF_STACK_OFF 76 /* offsetof(TCB_t, pxEndOfStack) */
#define TCB_IMPURE_PTR_OFF 92 /* offsetof(TCB_t, xNewLib_reent) */
#endif

#if 0
#define TCB_TOP_OF_STACK_OFF 0 /* offsetof(TCB_t, pxTopOfStack) */
#define TCB_MPU_SETTINGS_OFF 4 /* offsetof(TCB_t, xMPUSettings.mpumap) */
#define MPU_ENTRY_SIZE 8 /* sizeof(xthal_MPU_entry) */
#define MPU_ENTRY_AS_OFF 0 /* offsetof(xthal_MPU_entry, as) */
#define MPU_ENTRY_AT_OFF 4 /* offsetof(xthal_MPU_entry, at) */
#define TCB_TASK_NAME_OFF 132 /* offsetof(TCB_t, pcTaskName) */
#define TCB_END_OF_STACK_OFF 156 /* offsetof(TCB_t, pxEndOfStack) */
#define TCB_IMPURE_PTR_OFF 172 /* offsetof(TCB_t, xNewLib_reent) */
#endif

#endif /*  */
