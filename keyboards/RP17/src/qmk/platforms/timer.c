// Copyright 2023 Sergey Vlasov (@sigprof)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "timer.h"
#include <Arduino.h>

static __xdata uint32_t time_offset = 0;

void timer_init(void)
{
    time_offset = millis();
}

uint16_t timer_read(void)
{
    return (uint16_t)(timer_read32() & 0xFFFF);
}

uint32_t timer_read32(void)
{
    return millis() - time_offset;
}

uint32_t timer_elapsed32(uint32_t tlast)
{
    return TIMER_DIFF_32(timer_read32(), tlast);
}