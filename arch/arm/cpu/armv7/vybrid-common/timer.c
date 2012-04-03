/*
 * (C) Copyright 2007
 * Sascha Hauer, Pengutronix
 *
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/io.h>
#include <div64.h>
#include <asm/arch/timer.h>
#include <asm/arch/vybrid-regs.h>

DECLARE_GLOBAL_DATA_PTR;

#define timestamp (gd->tbl)

#define CONFIG_TMR_USEPIT
#ifdef CONFIG_TMR_USEPIT
static long long lastinc = 0;
static ulong timerticks = 0;

unsigned long long _usec2ticks(unsigned long long usec);

static inline unsigned long long tick_to_time(unsigned long long tick)
{
	tick /= timerticks;

	return tick;
}

int timer_init(void)
{
	ulong usecs;

	timestamp = 0;

	/*
	 * nsecs conversion = (1/ipg_clk) * 10^9
	 * equivalent to 1000 / (ipg_clk / 10^6)
	 */
	usecs = (ulong)((float)(gd->ipg_clk / 1000000));
	usecs = (ulong)((float)(1000 / usecs));

	clrbits_le32(PIT_MCR, 2);	/* enable PIT */

	/* 1 us per ticks = 1000 ns / usecs = cycles time */
	timerticks = (ulong)((float)(1000 / usecs));

	__raw_writel(0xFFFFFFFF, PIT_LDVAL1);
	__raw_writel(0, PIT_TCTRL1);
	__raw_writel(4, PIT_TCTRL1);
	__raw_writel(5, PIT_TCTRL1);
	__raw_writel(0xFFFFFFFF, PIT_LDVAL0);
	__raw_writel(1, PIT_TCTRL0);

	return 0;
}

ulong get_timer_masked(void)
{
	return tick_to_time(get_ticks());
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

/* delay x useconds AND preserve advance timstamp value */
void __udelay(unsigned long usec)
{
	ulong nsecs, tmp;

	/*
	 * nsecs conversion = (1/ipg_clk) * 10^9
	 * equivalent to 1000 / (ipg_clk / 10^6)
	 */
	nsecs = gd->ipg_clk / 1000000;
	nsecs = 1000 / nsecs;

	/* 1 us per ticks = 1000 ns / nsecs = cycles time */
	while (usec > 0) {
		if (usec > 65000)
			tmp = 65000;
		else
			tmp = usec;
		usec = usec - tmp;

		tmp =  (tmp * 1000) / nsecs;

		__raw_writel(tmp, PIT_LDVAL2);
		__raw_writel(1, PIT_TCTRL2);

		while ((__raw_readl(PIT_TFLG2) & 1) != 1);
		__raw_writel(0, PIT_TCTRL2);
		__raw_writel(1, PIT_TFLG2);
	}
}
#endif			/* CONFIG_TMR_USEPIT */

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
unsigned long long _usec2ticks(unsigned long long usec)
{
	usec *= timerticks;

	return usec;
}

unsigned long long get_ticks(void)
{
	long long now;

	now = (__raw_readl(PIT_LTMR64H) << 32);
	now |= __raw_readl(PIT_LTMR64L);

	timestamp = (lastinc - now);
	lastinc = now;

	return timestamp;
}

ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
