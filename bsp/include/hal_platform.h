/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _HAL_PLATFORM_
#define _HAL_PLATFORM_

#define HAL_READ32(base, addr)				(*((volatile u32*)(base + addr)))
#define HAL_WRITE32(base, addr, value32)		((*((volatile u32*)(base + addr))) = (value32))
#define HAL_READ16(base, addr)				(*((volatile u16*)(base + addr)))
#define HAL_WRITE16(base, addr, value)		((*((volatile u16*)(base + addr))) = (value))
#define HAL_READ8(base, addr)				(*((volatile u8*)(base + addr)))
#define HAL_WRITE8(base, addr, value)		((*((volatile u8*)(base + addr))) = value)


#ifdef __cplusplus
#define __I		volatile             /*!< Defines 'read only' permissions                 */
#else
#define __I		volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define __O		volatile             /*!< Defines 'write only' permissions                */
#define __IO	 	volatile             /*!< Defines 'read / write' permissions              */

/**************************************************************************//**
 * @defgroup AmebaD_Outline
 * @{
 *****************************************************************************/

/** @addtogroup AmebaD_Outline
  * @verbatim
  *****************************************************************************************
  * CPU
  *****************************************************************************************
  *	-Clock speed:		125MHz
  *	-Core:				CM4F
  *	-Working votage:	3.3V
  *
  *****************************************************************************************
  * Memory
  *****************************************************************************************
  *	-ROM:				512KB
  *	-RAM:				256KB
  *	-FLash:				128MB
  *
  *****************************************************************************************
  * GPIO
  *****************************************************************************************
  *	-GPIOA:				0~31, IN/OUT/INT
  *	-GPIOB:				0~31, IN/OUT/INT
  *
  *****************************************************************************************
  * Flash
  *****************************************************************************************
  *	-Max. speed:		100MHz
  *	-SPI Mode:			Quad IO mode
  *	-cache:				32K I/D read cache
  *	-XIP:				support executed in place
  *
  *****************************************************************************************
  * Firmware Protection
  *****************************************************************************************
  *	-RSIP(OTF):(Realsil Image Protection)
  *		-Flash Encryption
  *		-key store in OTP EFUSE Area, and can not read
  *	-RDP
  *		-4K RAM read protection
  *		-key store in OTP EFUSE Area, and can not read
  *
  *****************************************************************************************
  * UART
  *****************************************************************************************
  *	-Set count:			2
  *	-IrDA:				support
  *	-Low Power:			support
  *	-Sleep wakeup:		support
  *	-Baud rate:			110~6000000
  *	-GDMA:				support
  *
  *****************************************************************************************
  * UART LOG
  *****************************************************************************************
  *	-Set count:			1
  *	-IrDA:				support
  *	-Low Power:			not support
  *	-Sleep wakeup:		not support
  *	-Baud rate:			110~6000000
  *
  *****************************************************************************************
  * SPI Master
  *****************************************************************************************
  *	-Set count:			1
  *	-Max. Speed:		31.25MHz
  *	-GDMA:				support
  *
  *****************************************************************************************
  * SPI Slave
  *****************************************************************************************
  *	-Set count:			1
  *	-Max. Speed:		31.25MHz
  *	-GDMA:				support
  *
  *****************************************************************************************
  * I2C
  *****************************************************************************************
  *	-Set count:			1
  *	-Speed:				Standard (up to 100 kHz) and Fast (up to 400 kHz) Modes
  *	-Role:				Master & Slave
  *	-Sleep wakeup:		support
  *	-GDMA:				support
  *
  *****************************************************************************************
  * RTC
  *****************************************************************************************
  *	-Set count:			1
  *	-H:					12/24 hour mode
  *	-M:					0~59
  *	-S:					0~59
  *	-D:					0~511
  *	-Alarm:				support D/H/M/S
  *	-Daylight:			support
  *	-Calibration:		support
  *
  *****************************************************************************************
  * PWM
  *****************************************************************************************
  *	-Channel count:		6(KM0)/18(KM4)
  *	-Max. Resolution:	16bits (customizable)
  *	-Max. Frequency:	40MHz@1-bit mode, 40/(2^x))MHz@x-bit
  *
  *****************************************************************************************
  * Capture
  *****************************************************************************************
  *	-Channel count:		1
  *	-Max. Resolution:	16bits (customizable)
  *	-Max. Frequency:	40MHz@1-bit mode, 40/(2^x))MHz@x-bit
  *	-capture width:		supoort
  *	-capture number:	support
  *
  *****************************************************************************************
  * Timer
  *****************************************************************************************
  *	-Set count:			12
  *	-32KHz timer:		8
  *	-40MHz timer:		4
  *
  *****************************************************************************************
  * I2S
  *****************************************************************************************
  *	-Set count:			1
  *	-Source clk:		45.1584MHz or 98.304MHz(default)
  *	-Sample rate:		8K, 12K, 16K, 24K, 32K, 48K, 64K, 96K, 192K, 384K, 7.35K, 11.025K, 14.7K, 22.05K, 29.4K, 44.1K, 58.8K, 88.2K, 176.4K
  *	-Sample bit:		16 bit, 24 bit
  *	-DMA:				support
  *
  *****************************************************************************************
  * WIFI
  *****************************************************************************************
  *	-Set count:			1
  *	-Mode:				B/G/N(2.4G)
  *	-BW:				20MHz/40MHz
  *	-Ant:				1T/1R
  *
  *****************************************************************************************
  * USB device
  *****************************************************************************************
  *	-Set count:			1
  *	-DMA:				support
  *
  *****************************************************************************************
  * SDIO device
  *****************************************************************************************
  *	-Set count:			1
  *	-GSPI:				support
  *	-DMA:				support
  *
  *****************************************************************************************
  * BACKUP REG
  *****************************************************************************************
  *	-Set count:			4 dwords
  *
  *****************************************************************************************
  * CRYPTO
  *****************************************************************************************
  *	-MD5
  *	-SHA1
  *	-SHA2
  *	-HMAC-MD5
  *	-HMAC-SHA1
  *	-HMAC-SHA2
  *
  *	-AES-CBC
  *	-AES-ECB
  *	-AES-CTR
  *	-AES-CFB
  *	-AES-OFB
  *	-AES-GCM
  *	-3DES-CBC
  *	-3DES-ECB
  *	-DES-CBC
  *	-DES-ECB
  *
  *****************************************************************************************
  * ADC
  *****************************************************************************************
  *	-Set count:			1
  *	-Channel:			4
  *	-Sample rate:		max frequency up to 1MHz per channel, configurable
  *	-Resolution:		12 bit
  *	-signal sampling:	CH1 & CH3: 0 ~ 3V, CH2: 0~5V
  *	-power save:		Support one shot mode
  *	-GDMA:				support
  *
  *****************************************************************************************
  * GDMA
  *****************************************************************************************
  *	-Set count:			2
  *	-Channel:			2*6
  *
  *****************************************************************************************
  * @endverbatim
  */

/* TBD AutoGen */
#define SYSTEM_CTRL_BASE		0x4100C000

/* 1024B Retention Ram base address */
//#define RETENTION_RAM_BASE				0x000C0000
//#define RETENTION_RAM_SYS_OFFSET		0x80
//#define RETENTION_RAM_WIFI_FW_OFFSET	0x130 /* 0x000C_0130~0x000C_0400-1: 720B for wifi firmware */
//#define RETENTION_RAM_WIFI_FW_SIZE		720
/* TBD AutoGen */

/**************************************************************************//**
 * @defgroup AmebaD2_Address_Map AmebaD2 HS/LP Address Map
 * @{
 *****************************************************************************/
#define WIFI_REG_BASE 			 0x40010000
#define ZIGBEE_REG_BASE 		 0x42000000  		/* rxbuf:0x42000000, txbuf:0x42001000, zgb_cr:0x42001C00 */
#define RETENTION_RAM_BASE       0x4100CE00        /* ID:S6-4, Inter. Type:APB4, Top Address:0x4100CFFF, Size(KB):8K, Clk Domain:PERI_LCLK */
#define GDMA0_REG_BASE           0x40110000        /* ID:S5-4, Inter. Type:APB4, Top Address:0x40117FFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define SPORT0_REG_BASE          0x4011C000        /* ID:S5-9, Inter. Type:AHB, Top Address:0x4011CFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPORT1_REG_BASE          0x4011D000        /* ID:S5-10, Inter. Type:AHB, Top Address:0x4011DFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define UART0_REG_BASE           0x41010000        /* ID:S6-6, Inter. Type:APB4, Top Address:0x41010FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART1_REG_BASE           0x41011000        /* ID:S6-7, Inter. Type:APB4, Top Address:0x41011FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART2_REG_BASE           0x41012000        /* ID:S6-8, Inter. Type:APB4, Top Address:0x41012FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART3_REG_BASE           0x41013000        /* ID:S6-9, Inter. Type:APB4, Top Address:0x41013FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define AUDIO_REG_BASE           0x41017000        /* ID:S6-13, Inter. Type:APB4, Top Address:0x41017FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER0_REG_BASE          0x41018000        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER1_REG_BASE          0x41018200        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER2_REG_BASE          0x41018400        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER3_REG_BASE          0x41018600        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER4_REG_BASE          0x41018800        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER5_REG_BASE          0x41018A00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER6_REG_BASE          0x41018C00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER7_REG_BASE          0x41018E00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER8_REG_BASE          0x41019000        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER9_REG_BASE          0x41019200        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER10_REG_BASE         0x41019400        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER11_REG_BASE         0x41019600        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER12_REG_BASE         0x41019800        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER13_REG_BASE         0x41019A00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER14_REG_BASE         0x41019C00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define GPIO_REG_BASE            0x4101A000        /* ID:S6-16, Inter. Type:APB4, Top Address:0x4101AFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG0_REG_BASE            0x4100CC00        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG1_REG_BASE            0x4101F000        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG2_REG_BASE            0x4101F040        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG3_REG_BASE            0x4101F080        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG4_REG_BASE            0x4101F0C0        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC0_REG_BASE            0x41020000        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC1_REG_BASE            0x41020080        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC2_REG_BASE            0x41020100        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define PINMUX_REG_BASE          0x4100C800
#define GDMA0_REG_BASE_S         0x50110000        /* ID:S5-4, Inter. Type:APB4, Top Address:0x40117FFF, Size(KB):32K, Clk Domain:PERI_HCLK */
#define SPORT0_REG_BASE_S        0x5011C000        /* ID:S5-9, Inter. Type:AHB, Top Address:0x4011CFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define SPORT1_REG_BASE_S        0x5011D000        /* ID:S5-10, Inter. Type:AHB, Top Address:0x4011DFFF, Size(KB):4K, Clk Domain:PERI_HCLK */
#define UART0_REG_BASE_S         0x51010000        /* ID:S6-6, Inter. Type:APB4, Top Address:0x41010FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART1_REG_BASE_S         0x51011000        /* ID:S6-7, Inter. Type:APB4, Top Address:0x41011FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART2_REG_BASE_S         0x51012000        /* ID:S6-8, Inter. Type:APB4, Top Address:0x41012FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define UART3_REG_BASE_S         0x51013000        /* ID:S6-9, Inter. Type:APB4, Top Address:0x41013FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define AUDIO_REG_BASE_S         0x51017000        /* ID:S6-13, Inter. Type:APB4, Top Address:0x41017FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER0_REG_BASE_S        0x51018000        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER1_REG_BASE_S        0x51018200        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER2_REG_BASE_S        0x51018400        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER3_REG_BASE_S        0x51018600        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER4_REG_BASE_S        0x51018800        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER5_REG_BASE_S        0x51018A00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER6_REG_BASE_S        0x51018C00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER7_REG_BASE_S        0x51018E00        /* ID:S6-14, Inter. Type:APB4, Top Address:0x41018FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER8_REG_BASE_S        0x51019000        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER9_REG_BASE_S        0x51019200        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER10_REG_BASE_S       0x51019400        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER11_REG_BASE_S       0x51019600        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER12_REG_BASE_S       0x51019800        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER13_REG_BASE_S       0x51019A00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define TIMER14_REG_BASE_S       0x51019C00        /* ID:S6-15, Inter. Type:APB4, Top Address:0x41019FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define GPIO_REG_BASE_S          0x5101A000        /* ID:S6-16, Inter. Type:APB4, Top Address:0x4101AFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG0_REG_BASE_S          0x5100CC00        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG1_REG_BASE_S          0x5101F000        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG2_REG_BASE_S          0x5101F040        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG3_REG_BASE_S          0x5101F080        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define WDG4_REG_BASE_S          0x5101F0C0        /* ID:S6-21, Inter. Type:APB4, Top Address:0x4101FFFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC0_REG_BASE_S          0x51020000        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC1_REG_BASE_S          0x51020080        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */
#define IPC2_REG_BASE_S          0x510200F0        /* ID:S6-22, Inter. Type:APB4, Top Address:0x41020FFF, Size(KB):4K, Clk Domain:PERI_LCLK */

#define PINMUX_REG_BASE_S        0x5100C800

/** @} End of group AmebaD_Address_Map */

#define AON_BACKUP_BASE          0x4100C0E0
#define PMC_BASE                 0x4100C300

/**************************************************************************//**
 * @defgroup AmebaD_Peripheral_Declaration  AmebaD HS/LP Peripheral Declarations
 * @{
 *****************************************************************************/
#define RRAM_DEV			((RRAM_TypeDef			*) RETENTION_RAM_BASE)		/*retention mem */
#define UART0_DEV			((UART_TypeDef			*) UART0_REG_BASE)		/*hp uart0 */
#define UART1_DEV			((UART_TypeDef			*) UART1_REG_BASE)		/*hp uart1 */
#define UART2_DEV			((UART_TypeDef			*) UART2_REG_BASE)		/*hp uart2 */
#define UART3_DEV			((UART_TypeDef			*) UART3_REG_BASE)		/*hp uart3_bt*/
#define UART0_DEV_S			((UART_TypeDef			*) UART0_REG_BASE_S)		/*hp uart0 secure */
#define UART1_DEV_S			((UART_TypeDef			*) UART1_REG_BASE_S)		/*hp uart1 secure*/
#define UART2_DEV_S			((UART_TypeDef			*) UART2_REG_BASE_S)		/*hp uart2 secrue*/
#define UART3_DEV_S			((UART_TypeDef			*) UART3_REG_BASE_S)		/*hp uart3_bt secure*/

#define TIM0				((RTIM_TypeDef			*) TIMER0_REG_BASE)
#define TIM1				((RTIM_TypeDef			*) TIMER1_REG_BASE)
#define TIM2				((RTIM_TypeDef			*) TIMER2_REG_BASE)
#define TIM3				((RTIM_TypeDef			*) TIMER3_REG_BASE)
#define TIM4				((RTIM_TypeDef			*) TIMER4_REG_BASE)
#define TIM5				((RTIM_TypeDef			*) TIMER5_REG_BASE)
#define TIM6				((RTIM_TypeDef			*) TIMER6_REG_BASE)
#define TIM7				((RTIM_TypeDef			*) TIMER7_REG_BASE)
#define TIM8				((RTIM_TypeDef			*) TIMER8_REG_BASE)
#define TIM9				((RTIM_TypeDef			*) TIMER9_REG_BASE)
#define TIM10				((RTIM_TypeDef			*) TIMER10_REG_BASE)
#define TIM11				((RTIM_TypeDef			*) TIMER11_REG_BASE)
#define TIM12				((RTIM_TypeDef			*) TIMER12_REG_BASE)
#define TIM13				((RTIM_TypeDef			*) TIMER13_REG_BASE)
#define TIM14				((RTIM_TypeDef			*) TIMER14_REG_BASE)

#define TIM0_S				((RTIM_TypeDef			*) TIMER0_REG_BASE_S)
#define TIM1_S				((RTIM_TypeDef			*) TIMER1_REG_BASE_S)
#define TIM2_S				((RTIM_TypeDef			*) TIMER2_REG_BASE_S)
#define TIM3_S				((RTIM_TypeDef			*) TIMER3_REG_BASE_S)
#define TIM4_S				((RTIM_TypeDef			*) TIMER4_REG_BASE_S)
#define TIM5_S				((RTIM_TypeDef			*) TIMER5_REG_BASE_S)
#define TIM6_S				((RTIM_TypeDef			*) TIMER6_REG_BASE_S)
#define TIM7_S				((RTIM_TypeDef			*) TIMER7_REG_BASE_S)
#define TIM8_S				((RTIM_TypeDef			*) TIMER8_REG_BASE_S)
#define TIM9_S				((RTIM_TypeDef 			*) TIMER9_REG_BASE_S)
#define TIM10_S				((RTIM_TypeDef			*) TIMER10_REG_BASE_S)
#define TIM11_S				((RTIM_TypeDef			*) TIMER11_REG_BASE_S)
#define TIM12_S				((RTIM_TypeDef			*) TIMER12_REG_BASE_S)
#define TIM13_S				((RTIM_TypeDef			*) TIMER13_REG_BASE_S)
#define TIM14_S				((RTIM_TypeDef			*) TIMER14_REG_BASE_S)


#define PINMUX				((PINMUX_TypeDef		*) PINMUX_REG_BASE)			/*KM0 */
#define GPIOA_BASE			((GPIO_TypeDef			*) GPIO_REG_BASE)			/*KM0 */
#define GPIOB_BASE			((GPIO_TypeDef			*) (GPIO_REG_BASE + 0x400))	/*KM0 */
#define GPIOA_BASE_S		((GPIO_TypeDef			*) GPIO_REG_BASE_S)
#define GPIOB_BASE_S		((GPIO_TypeDef			*) (GPIO_REG_BASE_S + 0x400))
#define PINMUX_S			((PINMUX_TypeDef		*) PINMUX_REG_BASE_S)			/*KM0 */



#define IPCKR4_DEV 			((IPC_TypeDef 			*) IPC0_REG_BASE)				/*KR4 */
#define IPCKM4_DEV 			((IPC_TypeDef 			*) IPC1_REG_BASE)				/*KM4 */
#define IPCDSP_DEV 			((IPC_TypeDef			*) IPC2_REG_BASE)				/*DSP */
#define IPCKR4_DEV_S 			((IPC_TypeDef 			*) IPC0_REG_BASE_S)				/*KR4 Secure */
#define IPCKM4_DEV_S 			((IPC_TypeDef 			*) IPC1_REG_BASE_S)				/*KM4 Secure*/
#define IPCDSP_DEV_S 			((IPC_TypeDef			*) IPC2_REG_BASE_S)				/*DSP Secure*/

#define GDMA_BASE			((GDMA_TypeDef			*) (GDMA0_REG_BASE))

#define IWDG_DEV			((WDG_TypeDef			*) (WDG0_REG_BASE))
#define WDG1_DEV			((WDG_TypeDef			*) (WDG1_REG_BASE_S))
#define WDG2_DEV			((WDG_TypeDef			*) (WDG2_REG_BASE))
#define WDG3_DEV			((WDG_TypeDef			*) (WDG3_REG_BASE))
#define WDG4_DEV			((WDG_TypeDef			*) (WDG4_REG_BASE))


#define AUDIO_SPORT0_DEV	((AUDIO_SPORT_TypeDef	*) SPORT0_REG_BASE)
#define AUDIO_SPORT1_DEV	((AUDIO_SPORT_TypeDef	*) SPORT1_REG_BASE)
#define AUDIO_SPORT2_DEV	((AUDIO_SPORT_TypeDef	*) SPORT2_REG_BASE)
#define AUDIO_SPORT3_DEV	((AUDIO_SPORT_TypeDef	*) SPORT3_REG_BASE)
#define AUDIO_SPORT0_DEV_S	((AUDIO_SPORT_TypeDef	*) SPORT0_REG_BASE_S)	/*SPORT0 secure */
#define AUDIO_SPORT1_DEV_S	((AUDIO_SPORT_TypeDef	*) SPORT1_REG_BASE_S)	/*SPORT1 secure */

/** @} End of group AmebaD_Peripheral_Declaration */



/** @} End of group AmebaD_Outline */
#endif //_HAL_PLATFORM_
/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
