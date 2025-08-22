/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBAD2_LSYS_H_
#define _AMEBAD2_LSYS_H_


/**************************************************************************//**
 * @defgroup REG_LSYS_FEN_GRP0
 * @brief
 * @{
 *****************************************************************************/
#define APBPeriph_TIM14                            ((u32)(0  << 30) | (0x00000001 << 29))  /*!<R/W 0  1: enable TIMER 14 function 0: disable */
#define APBPeriph_TIM13                            ((u32)(0  << 30) | (0x00000001 << 28))  /*!<R/W 0  1: enable TIMER 13 function 0: disable */
#define APBPeriph_TIM12                            ((u32)(0  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: enable TIMER 12 function 0: disable */
#define APBPeriph_TIM11                            ((u32)(0  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: enable TIMER 11 function 0: disable */
#define APBPeriph_TIM10                            ((u32)(0  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: enable TIMER 10 function 0: disable */
#define APBPeriph_TIM_PWM                          ((u32)(0  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: enable PWM TIMER function 0: disable */
#define APBPeriph_TIM_PULSE                        ((u32)(0  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: enable PULSE TIMER function 0: disable */
#define APBPeriph_AC_AIP                           ((u32)(0  << 30) | (0x00000001 << 22))  /*!<R/W 0  1: enable audio codec analog ip active function, need level shift to 3.3V for audio aip 0: disable */
#define APBPeriph_SPORT1                           ((u32)(0  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: enable SPORT1 function 0: disable */
#define APBPeriph_SPORT0                           ((u32)(0  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: enable SPORT0 function 0: disable */
#define APBPeriph_AC                               ((u32)(0  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: enable audio codec digital ip active function 0: disable */

#define APBPeriph_TIM00                            ((u32)(1  << 30) | (0x00000001 << 12))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM01                            ((u32)(1  << 30) | (0x00000001 << 13))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM02                            ((u32)(1  << 30) | (0x00000001 << 14))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM03                            ((u32)(1  << 30) | (0x00000001 << 15))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM04                            ((u32)(1  << 30) | (0x00000001 << 16))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM05                            ((u32)(1  << 30) | (0x00000001 << 17))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM06                            ((u32)(1  << 30) | (0x00000001 << 18))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */
#define APBPeriph_TIM07                            ((u32)(1  << 30) | (0x00000001 << 19))  /*!<R/W 8'h0  Basic Timer0-7 function enable 1: enable 0: disable */

#define APBPeriph_TIM14_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 29))  /*!<R/W 0  1: Enable TIMER 14 clock 0: Disable */
#define APBPeriph_TIM13_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 28))  /*!<R/W 0  1: Enable TIMER 13 clock 0: Disable */
#define APBPeriph_TIM12_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 27))  /*!<R/W 0  1: Enable TIMER 12 clock 0: Disable */
#define APBPeriph_TIM11_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 26))  /*!<R/W 0  1: Enable TIMER 11 clock 0: Disable */
#define APBPeriph_TIM10_CLOCK                      ((u32)(0  << 30) | (0x00000001 << 25))  /*!<R/W 0  1: Enable TIMER 10 clock 0: Disable */
#define APBPeriph_TIM_PWM_CLOCK                    ((u32)(0  << 30) | (0x00000001 << 24))  /*!<R/W 0  1: Enable PWM TIMER clock 0: Disable */
#define APBPeriph_TIM_PULSE_CLOCK                  ((u32)(0  << 30) | (0x00000001 << 23))  /*!<R/W 0  1: Enable PULSE TIMER clock 0: Disable */
#define APBPeriph_SPORT1_CLOCK                     ((u32)(0  << 30) | (0x00000001 << 21))  /*!<R/W 0  1: Enable sport1 clock 0: Disable */
#define APBPeriph_SPORT0_CLOCK                     ((u32)(0  << 30) | (0x00000001 << 20))  /*!<R/W 0  1: Enable sport0 clock 0: Disable */
#define APBPeriph_AC_CLOCK                         ((u32)(0  << 30) | (0x00000001 << 19))  /*!<R/W 0  1: Enable audio codec clock 0: Disable */

#define APBPeriph_TIM_BASIC0_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 12))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */
#define APBPeriph_TIM_BASIC1_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 13))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */
#define APBPeriph_TIM_BASIC2_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 14))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */
#define APBPeriph_TIM_BASIC3_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 15))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */
#define APBPeriph_TIM_BASIC4_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 16))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */
#define APBPeriph_TIM_BASIC5_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 17))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */
#define APBPeriph_TIM_BASIC6_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 18))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */
#define APBPeriph_TIM_BASIC7_CLOCK                 ((u32)(1  << 30) | (0x00000001 << 19))  /*!<R/W 8'h00  LS basic Timer0-7 clk enable */



// /**************************************************************************//**
// * @defgroup REG_LSYS_CKSL_GRP0
// * @brief
// * @{
// *****************************************************************************/
#define LSYS_BIT_CKSL_LOGUART                      ((u32)0x00000001 << 29)          /*!<R/W 0  Select loguart trx clock 0: clock is xtal(source is dicided by cksl_loguart_xtal) 1: clock is osc2M */
#define LSYS_MASK_CKSL_UART                        ((u32)0x0000000F << 25)          /*!<R/W 0  Select uart rx clock for every uart . cksl_uart[n](n=0,1,2,3) 0: rx clock is xtal(source is dicided by cksl_uart_xtal) 1: rx clock is osc2M */
#define LSYS_CKSL_UART(x)                          ((u32)(((x) & 0x0000000F) << 25))
#define LSYS_GET_CKSL_UART(x)                      ((u32)(((x >> 25) & 0x0000000F)))
#define LSYS_BIT_CKSL_LOGUART_XTAL                 ((u32)0x00000001 << 24)          /*!<R/W 0  loguart_xtal clk select 0:clk_xtal_digi 1:clk_xtal_digi_div2 */
#define LSYS_MASK_CKSL_UART_XTAL                   ((u32)0x0000000F << 20)          /*!<R/W 0  uart xtal clk select, cksl_uart_xtal[n](n=0,1,2,3) 0: clk_xtal_digi 1:clk_xtal_digi_div2 */
#define LSYS_CKSL_UART_XTAL(x)                     ((u32)(((x) & 0x0000000F) << 20))
#define LSYS_GET_CKSL_UART_XTAL(x)                 ((u32)(((x >> 20) & 0x0000000F)))
#define LSYS_BIT_CKSL_SPORT                        ((u32)0x00000001 << 18)          /*!<R/W 1h  sport clock selection 0: equal to audio codec main clock (40M) 1: from cpu/dsp pll , after pll selection(cksl_i2s_pll) */
#define LSYS_BIT_CKSL_AC                           ((u32)0x00000001 << 17)          /*!<R/W 1h  audio codec main clock selection 0: cpupll divided 40M 1: xtal 40M */
#define LSYS_CKSL_AC(x)                            ((u32)(((x) & 0x00000001) << 17))
#define LSYS_BIT_CKSL_I2S_PLL                      ((u32)0x00000001 << 16)          /*!<R/W 0  source pll slection for sport clk 0: from cpu pll divider output 1: from dsp pll divider output */
#define LSYS_BIT_CKSL_VADM                         ((u32)0x00000001 << 15)          /*!<R/W 0  vad share memory clock select . Note, it must first change this bit to 1,and then shutdown hs plfm . When wakeup , it must first enable hs plfm , finally set this bit to 0 . 0: used by hs plfm clock 1: used by vad buffer ctrol */
#define LSYS_CKSL_VADM(x)                          ((u32)(((x) & 0x00000001) << 15))
#define LSYS_BIT_CKSL_CTC_CORE                     ((u32)0x00000001 << 12)          /*!<R/W 0h  CTC core clock selection 0: 131k 1: xtal2m divider Clock */
#define LSYS_CKSL_CTC_CORE(x)                      ((u32)(((x) & 0x00000001) << 12))
#define LSYS_BIT_CKSL_ADC2M                        ((u32)0x00000001 << 11)          /*!<R/W 0  lp adc 2m clk selection 0: divider from osc4m 1: divider from xtal */
#define LSYS_CKSL_ADC2M(x)                         ((u32)(((x) & 0x00000001) << 11))
#define LSYS_BIT_CKSL_CTC                          ((u32)0x00000001 << 10)          /*!<R/W 1h  CTC REG clock selection 0: 131k 1: lbus clock */
#define LSYS_CKSL_CTC(x)                           ((u32)(((x) & 0x00000001) << 10))
#define LSYS_BIT_CKSL_ADC                          ((u32)0x00000001 << 9)          /*!<R/W 0  ADC INT clock selection 0: lbus clock 1: OSC2M */
#define LSYS_CKSL_ADC(x)                           ((u32)(((x) & 0x00000001) << 9))
#define LSYS_BIT_CKSL_GPIO                         ((u32)0x00000001 << 8)          /*!<R/W 0  GPIO INT clock selecton: 0: lbus clock 1: 32.768K clock from SDM */
#define LSYS_CKSL_GPIO(x)                          ((u32)(((x) & 0x00000001) << 8))
#define LSYS_MASK_CKSL_PSRAM                       ((u32)0x00000003 << 6)          /*!<R/W 0  psram phy clock seleck cksl_psram[0]: 0--source clk is from cpu pll, 1--source clk is from dsp pll cksl_psram[1]: 0--final clk is from cpu/dsp pll divider 1: lbus clk 10/11: from lbus clock 00: from cpu pll divider output 01: from dsp pll divider output */
#define LSYS_CKSL_PSRAM(x)                         ((u32)(((x) & 0x00000003) << 6))
#define LSYS_MASK_CKSL_SPIC                        ((u32)0x00000003 << 4)          /*!<R/W 0  SPIC clock seleck 10/11: from lbus clock 00: from dsp pll divider output 01: from cpu pll divider output */
#define LSYS_CKSL_SPIC(x)                          ((u32)(((x) & 0x00000003) << 4))
#define LSYS_GET_HW_CHIP_TYPE(x)                   ((u32)(((x >> 8) & 0x00000003)))
#define LSYS_GET_HW_CPU_ID(x)                      ((u32)(((x >> 6) & 0x00000003)))

#define LSYS_BIT_EN_HIFI_RST_GLB                   ((u32)0x00000001 << 31)          /*!<R/W 0  1: if wdg of hifi assert wdg_reset, will trigger global reset directly. Follow the smart. 0: the wdg_reset of hifi wdg reset itself firstly, the global reset occurs only caused by timeout */

#define LSYS_BIT_EN_KR4_RST_GLB                    ((u32)0x00000001 << 30)          /*!<R/W 0  1: if wdg of hifi assert wdg_reset, will trigger global reset directly. Follow the smart. 0: the wdg_reset of hifi wdg reset itself firstly, if the kr4 is np, the global reset occurs only caused by timeout */

#define LSYS_BIT_EN_KM4_RST_GLB                    ((u32)0x00000001 << 29)          /*!<R/W 0  1: if wdg of hifi assert wdg_reset, will trigger global reset directly. Follow the smart. 0: the wdg_reset of hifi wdg reset itself firstly, if the km4 is np, the global reset occurs only caused by timeout */

#define LSYS_BIT_CLR_WDG1_RST                      ((u32)0x00000001 << 11)          /*!<WA0 0  write 1 to clear the km4_s_wdg_rst, glb_wdg_rst is not included */

#define LSYS_BIT_CLR_WDG2_RST                      ((u32)0x00000001 << 10)          /*!<WA0 0  write 1 to clear the km4_ns_wdg_rst, glb_wdg_rst is not included */

#define LSYS_BIT_CLR_WDG3_RST                      ((u32)0x00000001 << 9)          /*!<WA0 0  write 1 to clear the kr4_wdg_rst, glb_wdg_rst is not included */

#define LSYS_BIT_CLR_WDG4_RST                      ((u32)0x00000001 << 8)          /*!<WA0 0  write 1 to clear the dsp_wdg_rst, glb_wdg_rst is not included */

#define LSYS_MASK_WDG_TIMEOUT                      ((u32)0x000000FF << 0)          /*!<R/W 0  threshold of wdg timer, if wdg_timer >= wdg_timeout, will trigger global reset. Unit is 10us 0 means not support */
#define LSYS_WDG_TIMEOUT(x)                        ((u32)(((x) & 0x000000FF) << 0))
// /** @} */

/** @defgroup REG_LSYS_CKSL_GRP1
 * @brief
 * @{
 **/
#define LSYS_GET_CKSL_XTAL(x)                      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup REG_LSYS_SYSTEM_CFG1
 * @brief
 * @{
 **/
#define LSYS_BIT_KR4_IS_NP                         ((u32)0x00000001 << 1)          /*!<R/W 1  1: kr4 is Network Processor, and is the LPS controller 0: km4 is Network Processor, and is the LPS controller */
#define LSYS_KR4_IS_NP(x)                          ((u32)(((x) & 0x00000001) << 1))
#define LSYS_GET_KR4_IS_NP(x)                      ((u32)(((x >> 1) & 0x00000001)))

// /*==========LSYS Register Address Definition==========*/
#define REG_LSYS_FEN_GRP0                            0x0204
#define REG_LSYS_FEN_GRP1                            0x0208
#define REG_LSYS_CKE_GRP0                            0x020C
#define REG_LSYS_CKE_GRP1                            0x0210
#define REG_LSYS_CKSL_GRP0                           0x0218
#define REG_LSYS_CKSL_GRP1                           0x021C
#define REG_LSYS_SYSTEM_CFG0                         0x0228
#define REG_LSYS_SYSTEM_CFG1                         0x022C
#define REG_LSYS_WDG_GLBRST                          0x02C0



// /* MANUAL_GEN_START */
// //Please add your defination here

enum CHIP_TYPE {
	CHIP_TYPE_ASIC = 0,
	CHIP_TYPE_FPGA = 1,
	CHIP_TYPE_PALADIUM = 2,
	CHIP_TYPE_RTLSIM = 3
};

/**************************************************************************//**
 * @defgroup AMEBA_RRAM
 * @{
 * @brief AMEBA_RRAM Declaration
 * @ Total size of retention memory is 512Byte,Half retention mem resvd for system
 * @ the Max space for RRAM_TypeDef is 0x80, user can alloc space from RRAM_USER_RSVD
 *****************************************************************************/
typedef struct {
	__IO u8 RRAM_SYS_RSVD[256];/* resvd for system */

	__IO u32 IPC_Share_Mem; /* RETENTION_RAM_SYS_OFFSET 0x80 */
	__IO u32 PLLD_Clk_Info;

	__IO u32 IMQ_HEAP_ADDR[4];
	__IO u32 IMQ_HEAP_SIZE[4];
	__IO u32 IMQ_INIT_DONE;

	__IO u32 IMQ2_HEAP_ADDR;
	__IO u8 IMQ2_INIT_DONE;
	__IO u32 AP_WAKEUP_STATUS;
	__IO u32 PSRAM_CKSL_BK;

	__IO u8 PSRAM_TYPE;
	__IO u8 PSRAM_RESUMECNT_BOOT;
	__IO u8 CORE_Vol_Flag;

	__IO u8 SWRCALIB_AON_LDO_09;
	__IO u8 SWRCALIB_PWM_125; //store mode1 PWM125 autoload by HW, used by Reduce Voltage
	__IO u8 SWRCALIB_PWM_135; //store mode1 PWM135 prefetched from efuse, used by Raise Voltage
	__IO u8 SWRCALIB_CORE_LDO_09; //store CORE_LDO_09 autoload by HW, used by Reduce Voltage in mode1
	__IO u8 SWRCALIB_CORE_LDO_10; //store CORE_LDO_10 prefetched from efuse, used by Raise Voltage in mode1
	__IO u8 SWRCALIB_PWM_09; //store mode0 PWM09 autoload by HW, used by Reduce Voltage
	__IO u8 SWRCALIB_PWM_10; //store mode0 PWM10 prefetched from efuse, used by Raise Voltage

	__IO u8 PMC_CORE_ROLE_Flag;
	__IO u8 kr4_BackupTbl_Done;
	__IO u8 CHIP_INFO;
	__IO u32 psram_backup[6];

	__IO u32 DSPIMG_DTCM0_ADDR;
	__IO u32 DSPIMG_DTCM1_ADDR;

	__IO u8 RRAM_USER_RSVD[155];
} RRAM_TypeDef;
/** @} */

// /* MANUAL_GEN_END */

#endif

