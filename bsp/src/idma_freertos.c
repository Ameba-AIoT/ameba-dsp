/*
 * Copyright (c) 2019-2020 Cadence Design Systems. ALL RIGHTS RESERVED.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "xtensa/idma.h"

#include <xtensa/xtruntime.h>
#if XCHAL_HAVE_INTERRUPTS
#include <xtensa/tie/xt_interrupt.h>
#endif

#include "xtensa_api.h"

static ALIGNDCACHE idma_buf_t *buf_list[XCHAL_IDMA_NUM_CHANNELS];

/* Setup iDMA done & err interrupt handlers */

int32_t
idma_register_interrupts(int32_t ch, os_handler done_handler, os_handler err_handler)
{
#if XCHAL_HAVE_INTERRUPTS
	if (ch < XCHAL_IDMA_NUM_CHANNELS) {
		int32_t  ret     = 0;
		uint32_t doneint = (uint32_t) XCHAL_IDMA_CH0_DONE_INTERRUPT + (uint32_t) ch;
		uint32_t errint  = (uint32_t) XCHAL_IDMA_CH0_ERR_INTERRUPT + (uint32_t) ch;

		// Normally none of these calls should fail. We don't check them individually,
		// any one failing will cause this function to return failure.
//        ret += xtos_set_interrupt_handler(doneint, done_handler, cvt_int32_to_voidp(ch), NULL);
//        ret += xtos_interrupt_enable(doneint);
//
//        ret += xtos_set_interrupt_handler(errint, err_handler, cvt_int32_to_voidp(ch), NULL);
//        ret += xtos_interrupt_enable(errint);

		if (!xt_set_interrupt_handler(doneint, done_handler, cvt_int32_to_voidp(ch))) {
			ret += -1;
		}
		xt_interrupt_enable(doneint);

		if (!xt_set_interrupt_handler(errint, err_handler, cvt_int32_to_voidp(ch))) {
			ret += -1;
		}
		xt_interrupt_enable(errint);


		return ret;
	}

	return -1;
#endif
}

/* The following functions are provided as backups. The preferred versions are defined
   as inline functions in idma_os.h, but those take effect only if IDMA_APP_USE_XTOS is
   defined during application build. If that has not been defined then the non-inline
   versions below get linked in.
   NOTE: These *must* be identical to the inline versions defined in idma_os.h.
 */

uint32_t
idma_disable_interrupts(void)
{
//    return XTOS_SET_INTLEVEL(XCHAL_NUM_INTLEVELS);
	return (uint32_t)XT_RSIL(XCHAL_NUM_INTLEVELS);
}

void
idma_enable_interrupts(uint32_t level)
{
//    XTOS_RESTORE_INTLEVEL(level);
	XT_WSR_PS(level);
	XT_RSYNC();
}

void *
idma_thread_id(void)
{
	return NULL;
}

void
idma_thread_block(void *thread)  // parasoft-suppress MISRA2012-RULE-8_13_a-4 "Cannot use const in generic API"
{
	(void) thread;    // Unused

#if XCHAL_HAVE_XEA3
	(void) XTOS_DISABLE_INTERRUPTS();
	(void) XTOS_SET_INTLEVEL(0);
#endif
	XT_WAITI(0);
}

void
idma_thread_unblock(void *thread)  // parasoft-suppress MISRA2012-RULE-8_13_a-4 "Cannot use const in generic API"
{
	(void) thread;    // Unused
}

void
idma_chan_buf_set(int32_t ch, idma_buf_t *buf)
{
	if (ch < XCHAL_IDMA_NUM_CHANNELS) {
//        g_idma_buf_ptr[ch] = buf;
		buf_list[ch] = buf;
	}
}

idma_buf_t *
idma_chan_buf_get(int32_t ch)
{
	if (ch < XCHAL_IDMA_NUM_CHANNELS) {
//        return g_idma_buf_ptr[ch];
		return buf_list[ch];
	}
	return NULL;
}

