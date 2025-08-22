/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

u32 ConfigDebugClose = 0;
char g_print_buf[512];

#define UART_LOG_IRQ_KR4 34
#define UART_LOG_IRQ 35
#define DiagPutChar LOGUART_PutChar


static LOG_UART_PORT LOG_UART_IDX_FLAG[] = {
	{1, LOGUART_BIT_TP2F_NOT_FULL, LOGUART_BIT_TP2F_EMPTY, 52125, UART_LOG_IRQ_KR4},	/* CM0 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{0, LOGUART_BIT_TP1F_NOT_FULL, LOGUART_BIT_TP1F_EMPTY, 781875, UART_LOG_IRQ},		/* CM4 IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
	{3, LOGUART_BIT_TP4F_NOT_FULL, LOGUART_BIT_TP4F_EMPTY, 3127500, 0},	/* DSP IDX NOT_FULL EMPTY TX_TIMEOUT IRQ*/
};

int32_t g_cnt = 0;
void LOGUART_PutChar(u8 c)
{
#if 1
	LOGUART_TypeDef *UARTLOG = LOGUART_DEV;
	u32 CounterIndex = 0;
	u32 CPUID = 2;//SYS_CPUID();

	if (ConfigDebugClose == 1) {
		return;
	}

	while (1) {
#if 0
		CounterIndex++;
		if (CounterIndex >= LOG_UART_IDX_FLAG[CPUID].tx_timeout) {
			break;
		}
#endif
		if (UARTLOG->LOGUART_UART_LSR & LOG_UART_IDX_FLAG[CPUID].not_full) {
			break;
		}
	}

	UARTLOG->LOGUART_UART_THRx[LOG_UART_IDX_FLAG[CPUID].idx] = c;

	if (c == KB_ASCII_LF) {
		UARTLOG->LOGUART_UART_THRx[LOG_UART_IDX_FLAG[CPUID].idx] = KB_ASCII_CR;
	}
#endif
}

int DiagVSprintf(char *buf, const char *fmt, va_list ap)
{
	char *p, *s;
	const va_list *dp = &ap;
	s = buf;

	for (; *fmt != '\0'; ++fmt) {
		if (*fmt != '%') {
			if (buf) {
				*s++ = *fmt;
			} else {
				DiagPutChar(*fmt);
			}

			continue;
		}

		if (*++fmt == 's') {
#ifndef DSP_TODO
			int val = va_arg(ap, int);

			p = (char *)val;
			for (p = (char *)val; *p != '\0'; p++) {
				if (buf) {
					*s++ = *p;
				} else {
					DiagPutChar(*p);
				}
			}
#endif
		} else {	/* Length of item is bounded */
			char tmp[20], *q = tmp;
			int alt = 0;
			int shift = 0;// = 12;
			int arg_val = va_arg(ap, int);
			if ((arg_val) < 0x10) {
				shift = 0;
			} else if (((arg_val) >= 0x10) && ((arg_val) < 0x100)) {
				shift = 4;
			} else if (((arg_val) >= 0x100) && ((arg_val) < 0x1000)) {
				shift = 8;
			} else if (((arg_val) >= 0x1000) && ((arg_val) < 0x10000)) {
				shift = 12;
			} else if (((arg_val) >= 0x10000) && ((arg_val) < 0x100000)) {
				shift = 16;
			} else if (((arg_val) >= 0x100000) && ((arg_val) < 0x1000000)) {
				shift = 20;
			} else if (((arg_val) >= 0x1000000) && ((arg_val) < 0x10000000)) {
				shift = 24;
			} else {
				shift = 28;
			}
#if 1   //wei patch for %02x

			if ((*fmt  >= '0') && (*fmt  <= '9')) {
				int width;
				unsigned char fch = *fmt;

				for (width = 0; (fch >= '0') && (fch <= '9'); fch = *++fmt) {
					width = width * 10 + fch - '0';
				}

				shift = (width - 1) * 4;
			}

#endif

			/*
			 * Before each format q points to tmp buffer
			 * After each format q points past end of item
			 */

			if ((*fmt == 'x') || (*fmt == 'X') || (*fmt == 'p') || (*fmt == 'P')) {
				/* With x86 gcc, sizeof(long) == sizeof(int) */
#ifndef DSP_TODO

				const long *lp = (const long *)dp;
				unsigned long h = (unsigned long)arg_val;
				int hex_count = 0;
				unsigned long h_back = (unsigned long)arg_val;
				int ncase = (*fmt & 0x20);
				//dp = arg_val;

				if ((*fmt == 'p') || (*fmt == 'P')) {
					alt = 1;
				}

				if (alt) {
					*q++ = '0';
					*q++ = 'X' | (char)ncase;
				}


				while (h_back) {
					hex_count += 1;
					h_back  = h_back >> 4;
				}


				if (shift < (hex_count - 1) * 4) {
					shift = (hex_count - 1) * 4;
				}

				//printf("(%d,%d)", hex_count, shift);

				for (; shift >= 0; shift -= 4) {

					*q++ = "0123456789ABCDEF"[(h >> shift) & 0xF] | (char)ncase;
				}
#endif
			} else if (*fmt == 'd') {
				int i = arg_val;
				char *r;
				int digit_space = 0;

				if (i < 0) {
					*q++ = '-';
					i = -i;
					digit_space++;
				}

				p = q;		/* save beginning of digits */

				do {
					*q++ = '0' + (i % 10);
					i /= 10;
					digit_space++;
				} while (i);


				for (; shift >= 0; shift -= 4) {

					if (digit_space-- > 0) {
						; //do nothing
					} else {
						*q++ = '0';
					}
				}

				/* reverse digits, stop in middle */
				r = q;		/* don't alter q */

				while (--r > p) {
					i = *r;
					*r = *p;
					*p++ = (char)i;
				}
			} else if (*fmt == 'c') {
				*q++ = (char)arg_val;
			} else {
				*q++ = *fmt;
			}

			/* now output the saved string */
			for (p = tmp; p < q; ++p) {
				if (buf) {
					*s++ = *p;
				} else {
					DiagPutChar(*p);
				}

				if ((*p) == '\n') {
					DiagPutChar('\r');
				}
			}
		}
	}

	if (buf) {
		*s = '\0';
	}

	return (s - buf);
}

/* outbyte is called by printf at last*/
int outbyte(char c)
{
	LOGUART_PutChar(c);
	return 1;
}

/* inbyte should also be included to avoid linking functions in libminrt.a*/
int inbyte(void)
{
	return -1;		/* always EOF */
}

int32_t DiagPrintf(const char *fmt, ...)
{
	int ret = 0;
	int para;
	va_list ap;

	if (ConfigDebugClose == 1) {
		return 0;
	}


	va_start(ap, fmt);
	//para = va_arg(ap, int);
	ret = DiagVSprintf(NULL, fmt, ap);
	va_end(ap);

	return ret;
}

