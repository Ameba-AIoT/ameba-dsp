/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <xtensa/xt_profiling.h>
#include "xtensa_api.h"

void xt_profile_set_interrupt_handler(uint32_t interrupt, void * handler)
{
    if (NULL ==  xt_set_interrupt_handler(interrupt, handler, NULL)) {
        // profiling interrupt can not be at highpri interrupt.
        exit(-1);
    }
}

void xt_profile_interrupt_on(uint32_t interrupt)
{
    (void) xt_interrupt_enable(interrupt);
}

void xt_profile_interrupt_off(uint32_t interrupt)
{
    (void) xt_interrupt_disable(interrupt);
}
