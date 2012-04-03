/*
 * (C) Copyright 2012
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
 *
 * Configuration settings for the vybrid Board
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define DEBUG

 /* High Level Configuration Options */

#define CONFIG_VYBRID

#define CONFIG_SYS_VYBRID_HCLK		24000000
#define CONFIG_SYS_VYBRID_CLK32		32768
#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

#define CONFIG_SYS_ICACHE_OFF

#include <asm/arch/vybrid-regs.h>
/*
 * Disabled for now due to build problems under Debian and a significant
 * increase in the final file size: 144260 vs. 109536 Bytes.
 */

#undef CONFIG_CMDLINE_TAG			/* enable passing of ATAGs */
#undef CONFIG_SETUP_MEMORY_TAGS
#undef CONFIG_INITRD_TAG

#undef CONFIG_OF_LIBFDT

#define CONFIG_MACH_TYPE		MACH_TYPE_MX51_BABBAGE
/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(4 << 10)

#define CONFIG_BOARD_LATE_INIT

/* Hardware drivers */
#define CONFIG_VYBRID_UART
#define CONFIG_VYBRID_UART_BASE		UART0_BASE
#define CONFIG_VYBRID_GPIO

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_UART_PORT		(1)
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{9600, 19200, 38400, 57600, 115200}

/* Command definition */
#undef CONFIG_CMD_BDI		/* bdinfo */
#undef CONFIG_CMD_BOOTD
#undef CONFIG_CMD_BOOTM
#define CONFIG_CMD_CONSOLE	/* coninfo */
#undef CONFIG_CMD_FPGA		/* FPGA configuration Support */
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
//#define CONFIG_CMD_MISC
#undef CONFIG_CMD_RUN		/* run command in env variable */
#undef CONFIG_CMD_SOURCE	/* "source" command support */
#undef CONFIG_CMD_XIMG		/* Load part of Multi Image */

#undef CONFIG_CMD_IMI		/* iminfo */
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_LOADB		/* loadb */
#undef CONFIG_CMD_LOADS		/* loads */

#undef CONFIG_CMD_DATE

#define CONFIG_BOOTDELAY	3

#define CONFIG_ETHPRIME		"FEC0"

#define CONFIG_LOADADDR		0x3f400000	/* loadaddr env var */

#define CONFIG_ARP_TIMEOUT	200UL

/* Miscellaneous configurable options */
#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT		"VyBrid U-Boot > "
#undef CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_CBSIZE		256	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS	16	/* max number of command args */
#define CONFIG_SYS_BARGSIZE CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

#define CONFIG_SYS_MEMTEST_START       0x3f400000
#define CONFIG_SYS_MEMTEST_END         0x10000

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

#define CONFIG_SYS_HZ		1000
#undef CONFIG_CMDLINE_EDITING

/*
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128 * 1024)	/* regular stack */

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS	1
#define PHYS_SDRAM_1		CSD0_BASE_ADDR
#define PHYS_SDRAM_1_SIZE	(512 * 1024 * 1024)

//#define CONFIG_SYS_SDRAM_BASE		(PHYS_SDRAM_1)
//#define CONFIG_SYS_SDRAM_BASE		(CONFIG_SYS_INIT_RAM_ADDR)
#define CONFIG_SYS_SDRAM_BASE		(0x3f400000)
#define CONFIG_SYS_INIT_RAM_ADDR	(IRAM_BASE_ADDR)
#define CONFIG_SYS_INIT_RAM_SIZE	(IRAM_SIZE)

#define CONFIG_BOARD_EARLY_INIT_F

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_SYS_DDR_CLKSEL	0
#define CONFIG_SYS_CLKTL_CBCDR	0x59E35100

/* timer */
#define FTM_BASE_ADDR			FTM0_BASE_ADDR
#define CONFIG_TMR_USEPIT

/* clock/PLL configuration */
#define CONFIG_SYS_CLKCTL_CCGR0		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR1		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR2		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR3		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR4		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR5		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR6		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR7		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR8		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR9		0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR10	0xFFFFFFFF
#define CONFIG_SYS_CLKCTL_CCGR11	0xFFFFFFFF

#define CONFIG_SYS_CLKCTRL_CCR		0x00010005
#define CONFIG_SYS_CLKCTRL_CCSR		0x0003FF24
#define CONFIG_SYS_CLKCTRL_CACRR	0x00000810
#define CONFIG_SYS_CLKCTRL_CSCMR1	0x00000000
#define CONFIG_SYS_CLKCTRL_CSCDR1	0x00000000
#define CONFIG_SYS_CLKCTRL_CSCDR2	0x00000000
#define CONFIG_SYS_CLKCTRL_CSCDR3	0x00000000
#define CONFIG_SYS_CLKCTRL_CSCMR2	0x00000000
#define CONFIG_SYS_CLKCTRL_CSCDR4	0x00000000

/* FLASH and environment organization */
#define CONFIG_SYS_NO_FLASH

#define CONFIG_ENV_OFFSET      (6 * 64 * 1024)
#define CONFIG_ENV_SIZE        (8 * 1024)
#undef CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_SYS_MMC_ENV_DEV 0

#endif
