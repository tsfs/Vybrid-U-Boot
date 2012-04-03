/*
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

#ifndef __ARCH_ARM_MACH_VYBRID_CRM_REGS_H__
#define __ARCH_ARM_MACH_VYBRID_CRM_REGS_H__

/* DPLL register mapping structure */
struct mxc_pll_reg {
	u32 ctrl;
	u32 config;
	u32 op;
	u32 mfd;
	u32 mfn;
	u32 mfn_minus;
	u32 mfn_plus;
	u32 hfs_op;
	u32 hfs_mfd;
	u32 hfs_mfn;
	u32 mfn_togc;
	u32 destat;
};

/* Register maping of CCM*/
struct mxc_ccm_reg {
};

#endif				/* __ARCH_ARM_MACH_VYBRID_CRM_REGS_H__ */
