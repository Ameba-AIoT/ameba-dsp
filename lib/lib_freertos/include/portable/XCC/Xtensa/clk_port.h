#ifndef _CLK_PORT_H_
#define _CLK_PORT_H_

#define REG_LSYS_SYSTEM_CFG0        0x0228
#define SYSTEM_CTRL_BASE        0x4100C000
#define LSYS_GET_HW_CHIP_TYPE(x)        ((unsigned int)(((x >> 8) & 0x00000003)))
unsigned int PLL_ClkGet(unsigned char pll_type);
#define CLK_CPU_DPLL		3

#endif