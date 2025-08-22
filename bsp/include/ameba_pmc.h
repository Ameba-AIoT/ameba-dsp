/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_PMC_H_
#define _AMEBA_PMC_H_

/** @addtogroup AmebaD_Platform
  * @{
  */

/** @defgroup PMC
  * @brief PMC driver modules
  * @{
  */

/** @addtogroup PMC
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * we support  following soc power save functions:
  *		- sleep clock gating
  *		- sleep power gating
  *		- deep standby
  *		- deep sleep
  *
  *****************************************************************************************
  * sleep power gating
  *****************************************************************************************
  * following functions can be used when power gating:
  *		-UART0/UART1
  *		-TIM4/TIM5
  *		-RTC
  *		-WIFI
  *		-SDIO
  *		-USB
  *		-I2C0/I2C1
  *		-ADC
  *		-GPIO
  *		-REGU timer
  *		-normal wakepin
  *		-ANA timer
  * following functions will be closed when power gating:
  *		-UART2 LOGUART
  *		-TIM0-TIM3
  *		-SPIC flash
  *
  *****************************************************************************************
  * deep standby
  *****************************************************************************************
  * following functions can be used when deep standby:
  *		-RTC
  *		-REGU timer
  *		-normal wakepin
  *		-ANA timer
  *
  *****************************************************************************************
  * deep sleep
  *****************************************************************************************
  * following functions can be used when deep standby:
  *		-REGU timer
  *		-REGU wakepin
  *
  *****************************************************************************************
  * wakepin (A18/A5/A22/A23: mux normal wakepin and REGU wakepin)
  *****************************************************************************************
  * normal wakepin:
  *		-SLP_CG
  *		-SLP_PG
  *		-STDBY
  * REGU wakepin:
  *		-just used in DSLP (1.2V closed)
  *		-just support high acive, so this pin should pull low on your board
  *
  *****************************************************************************************
  *****************************************************************************************
  * SLP & SNZ power option
  *****************************************************************************************
  * BIT_SYSON_PMOPT_SLP_EN_SWR & BIT_SYSON_PMOPT_SNZ_EN_SWR
  *		-we have two 1.2V LDO
  *			-BIG LDO: SWR mode or LDO mode (can config )
  *			-LITTLE LDO: a little 1.2v LDO
  *		-BIT_SYSON_PMOPT_SLP_EN_SWR
  *			-ENABLE/DISABLE BIG LDO when SLP
  *		BIT_SYSON_PMOPT_SNZ_EN_SWR
  *			-ENABLE/DISABLE BIG LDO when SNZ, WIFI & ADC need open BIG LDO when SNZ
  *
  * BIT_SYSON_PMOPT_SLP_EN_PWM & BIT_SYSON_PMOPT_SNZ_EN_PWM
  *		-BIT_SYSON_PMOPT_SLP_EN_PWM
  *			-ENABLE/DISABLE LDO heavy loading current mode when SLP
  *		-BIT_SYSON_PMOPT_SNZ_EN_PWM
  *			-ENABLE/DISABLE heavy loading current mode when SNZ, WIFI & ADC need heavy loading when SNZ
  *
  * BIT_SYSON_PMOPT_SLP_XTAL_EN & BIT_SYSON_PMOPT_SNZ_XTAL_EN
  *		-WIFI and SOC both need XTAL when work,
  *		-but WIFI have individual option to control XTAL, so BIT_SYSON_PMOPT_SNZ_XTAL_EN not needed
  *
  * BIT_SYSON_PMOPT_SLP_SYSPLL_EN & BIT_SYSON_PMOPT_SNZ_SYSPLL_EN
  *		-WIFI and SOC both have individual PLL, here is SOC 500M PLL
  *		-So BIT_SYSON_PMOPT_SNZ_SYSPLL_EN not needed
  *
  * BIT_SYSON_SNFEVT_WIFI_MSK = 1 & BIT_SYSON_BYPASS_SNZ_SLP = 1
  *		- after OS suspend, platform will enter SNZ and close CPU, then platform enter sleep mode when WIFI 32K
  *		- BIT_SYSON_PMOPT_SNZ_EN_SOC should never open, or CPU will not close when platform will enter SNZ
  *
  * BIT_SYSON_SNFEVT_WIFI_MSK = 1 & BIT_SYSON_BYPASS_SNZ_SLP = 0 (not use this config)
  *		- after OS suspend, platform will enter sleep mode & close CPU after WIFI 32K
  *****************************************************************************************
  * @endverbatim
  */

/** @defgroup sleep_parameter np & ap sleep parameter
  * @{
  */
#ifndef CONFIG_BUILD_ROM

typedef struct {
	u32	sleep_type;
	u32	sleep_time;
	u32	dlps_enable;
	u32	rsvd[5];
} SLEEP_ParamDef;

#endif
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup PMC_Exported_Constants PMC Exported Constants
  * @{
  */


/** @defgroup NP_WAKE_AP by IPC list
  * @{
  */
#define FW_NPWAP_IPC			0
#define TIMER_NPWAP_IPC			1

extern void SOCPS_SetDSPWakeEvent_MSK0(u32 Option, u32 NewStatus);
extern void SOCPS_SetDSPWakeEvent_MSK1(u32 Option, u32 NewStatus);
extern void SOCPS_SetDSPWakeEvent(u32 Option, u32 Group, u32 NewStatus);

void SOCPS_SleepCG(void);

void SOCPS_NPWDSP_ipc_int(void *Data, u32 IrqStatus, u32 ChanNum);
void SOCPS_APWDSP_ipc_int(void *Data, u32 IrqStatus, u32 ChanNum);


#endif //_AMEBA_PMC_H_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
