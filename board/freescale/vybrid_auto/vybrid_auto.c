/*
 * (C) Copyright 2012
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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
#include <asm/fec.h>
#include <asm/gpio.h>
#include <asm/arch/vybrid-regs.h>
#include <asm/arch/vybrid-pins.h>
#include <asm/arch/iomux.h>
#include <asm/errno.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/crm_regs.h>
#include <i2c.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <usb/ehci-fsl.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_FSL_ESDHC
struct fsl_esdhc_cfg esdhc_cfg[2] = {
	{ESDHC1_BASE_ADDR, 1},
	{ESDHC2_BASE_ADDR, 1},
};
#endif

void ddr_phy_init(void)
{
#define PHY_DQ_TIMING		0x00002613
#define PHY_DQS_TIMING		0x00002615
#define PHY_CTRL		0x01210080
#define PHY_MASTER_CTRL		0x0001012a
#define PHY_SLAVE_CTRL		0x00012020

	/* phy_dq_timing_reg freq set 0 */
	__raw_writel(PHY_DQ_TIMING, DDR_PHY000);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY016);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY032);
	__raw_writel(PHY_DQ_TIMING, DDR_PHY048);

	/* phy_dqs_timing_reg freq set 0 */
	__raw_writel(PHY_DQS_TIMING, DDR_PHY001);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY017);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY033);
	__raw_writel(PHY_DQS_TIMING, DDR_PHY049);

	/* phy_gate_lpbk_ctrl_reg freq set 0 */
	__raw_writel(PHY_CTRL, DDR_PHY002);	// read delay bit21:19
	__raw_writel(PHY_CTRL, DDR_PHY018);	// phase_detect_sel bit18:16
	__raw_writel(PHY_CTRL, DDR_PHY034);	// bit lpbk_ctrl bit12
	__raw_writel(PHY_CTRL, DDR_PHY050);

	/* phy_dll_master_ctrl_reg freq set 0 */
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY003);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY019);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY035);
	__raw_writel(PHY_MASTER_CTRL, DDR_PHY051);

	/* phy_dll_slave_ctrl_reg freq set 0 */
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY004);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY020);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY036);
	__raw_writel(PHY_SLAVE_CTRL, DDR_PHY052);

	__raw_writel(0x00001100, DDR_PHY050);
}

unsigned long ddr_ctrl_init(void)
{
	int i, dram_size = 0x8000000;

	__raw_writel(0x00000600, DDR_CR000);	/* LPDDR2 or DDR3 */
	__raw_writel(0x00000020, DDR_CR002);	/* TINIT */
	__raw_writel(0x0000007c, DDR_CR010);	/* reset during power on */
						/* warm boot - 200ns */
	__raw_writel(0x00013880, DDR_CR011);	/* 500us - 10ns */
	__raw_writel(0x0000050c, DDR_CR012);	/* CASLAT_LIN, WRLAT */
	__raw_writel(0x15040404, DDR_CR013);	/* trc, trrd, tccd
						   tbst_int_interval */
	__raw_writel(0x1406040F, DDR_CR014);	/* tfaw, tfp, twtr, tras_min */
	__raw_writel(0x04040000, DDR_CR016);	/* tmrd, trtp */
	__raw_writel(0x006DB00C, DDR_CR017);	/* tras_max, tmod */
	__raw_writel(0x00000403, DDR_CR018);	/* tckesr, tcke */

	__raw_writel(0x01000403, DDR_CR020);	/* ap, writeinterp, tckesr
						   tcke */
	__raw_writel(0x06060101, DDR_CR021);	/* trcd_int, tras_lockout
						   ccAP */
	__raw_writel(0x000B0000, DDR_CR022);	/* tdal */
	__raw_writel(0x03000200, DDR_CR023);	/* bstlen, tmrr, tdll */
	__raw_writel(0x00000006, DDR_CR024);	/* addr_mirror, reg_dimm
						   trp_ab_f0 */
	__raw_writel(0x00010000, DDR_CR025);	/* tref_enable, auto_refresh
						   arefresh */
	__raw_writel(0x06180016, DDR_CR026);	/* tref_f0, trfc_f0 */
	__raw_writel(0x00000005, DDR_CR028);	/* tref_interval fixed at 5 */
	__raw_writel(0x00020002, DDR_CR029);	/* tpdex_f1 and tpdex_f0 */

	__raw_writel(0x00050005, DDR_CR030);	/* txpdll_f1, txpdll_f0 */
	__raw_writel(0x00180200, DDR_CR031);	/* txsnr_f0, txsr_f0 */
	__raw_writel(0x00180200, DDR_CR032);	/* txsnr_f1, txsr_f1 */
	__raw_writel(0x00010000, DDR_CR033);	/* cke_dly, en_quick_srefresh
						 * srefresh_exit_no_refresh,
						 * pwr, srefresh_exit
						 */
	__raw_writel(0x02020200, DDR_CR034);	/* ckesre_f1, cksrx_f0, */
						/* cksre_f0, lowpwr_ref_en */
	__raw_writel(0x00000002, DDR_CR035);	/* lp..., cksrx_f1 */

	/* Frequency change */
	__raw_writel(0x00000100, DDR_CR038);	/* freq change... */
	__raw_writel(0x04001002, DDR_CR039);

	__raw_writel(0x00040010, DDR_CR040);
	__raw_writel(0x00000101, DDR_CR041);
	__raw_writel(0x00000140, DDR_CR042);
	__raw_writel(0x00000000, DDR_CR043);
	__raw_writel(0x00000001, DDR_CR044);
	__raw_writel(0x00000000, DDR_CR045);
	__raw_writel(0x00000000, DDR_CR046);
	__raw_writel(0x00000000, DDR_CR047);	/* REF_PER_AUTO_TEMPCHK:
						 *   LPDDR2 set to 2, else 0
						 */

	/* DRAM device Mode registers */
	__raw_writel(0x00460220, DDR_CR048);	/* mr0_f0_0, ddr3 burst of 8 only
						 * mr1_f0_0, if freq < 125
						 * dll_dis = 1, rtt = 0
						 * if freq > 125, dll_dis = 0
						 * rtt = 3
						 */
	__raw_writel(0x02200000, DDR_CR049);	/* mr0_f1_0 & mr2_f0_0 */

	__raw_writel(0x00080003, DDR_CR050);	/* mr2_f1_0, mr1_f1_0 */
	__raw_writel(0x00000000, DDR_CR051);	/* mr3_0 & mrsingle_data_0 */
	__raw_writel(0x00460410, DDR_CR053);	/* mr1_f0_1, mr0_f0_1 */
	__raw_writel(0x02200000, DDR_CR054);	/* mr0_f1_1, mr2_f0_1 */
	__raw_writel(0x00080003, DDR_CR055);	/* mr2_f1_1, mr1_f1_1 */
	__raw_writel(0x02000000, DDR_CR057);	/* ctrl_raw, mr17, mr16, mr8 */
						/* ctrl_raw, if DDR3, set 3
						 * else 0 */

	/* ECC */
	//__raw_writel(0x01000000, DDR_CR058);

	/* ZQ stuff */
	__raw_writel(0x01000200, DDR_CR066);	/* zqcl_f0, zqinit_f0 */
	__raw_writel(0x02000040, DDR_CR067);	/* zqinit_f1, zqcs_f0 */
	__raw_writel(0x00400100, DDR_CR068);	/* zqcs_f1, zqcl_f1 */
	__raw_writel(0x00000020, DDR_CR069);	/* zq_on_sref_exit, qz_req */

	__raw_writel(0x00000040, DDR_CR070);	/* ref_per_zq */
	__raw_writel(0x00000000, DDR_CR071);	/* zqreset_f0, ddr3 set to 0 */
	__raw_writel(0x01000000, DDR_CR072);	/* zqcs_rotate, no_zq_init
						   zqreset_f1 */

	/* DRAM controller misc */
	__raw_writel(0x0a010200, DDR_CR073);	/* arebit, col_diff, row_diff
						   bank_diff */
	__raw_writel(0x0101ffff, DDR_CR074);	/* bank_split, addr_cmp_en
						   cmd/age cnt */
	__raw_writel(0x01010101, DDR_CR075);	/* rw same pg, rw same en
						   pri en, plen */
	__raw_writel(0x03030101, DDR_CR076);	/* #q_entries_act_dis
						 * (#cmdqueues
						 * dis_rw_grp_w_bnk_conflict
						 * w2r_split_en, cs_same_en */
	__raw_writel(0x01000101, DDR_CR077);	/* cs_map, inhibit_dram_cmd
						 * dis_interleave, swen */
	__raw_writel(0x0000000c, DDR_CR078);	/* qfull, lpddr2_s4, reduc
						   burst_on_fly */
	__raw_writel(0x01000000, DDR_CR079);	/* ctrlupd_req_per aref en
						 * ctrlupd_req
						 * ctrller busy
						 * in_ord_accept */

	/* ODT */
	__raw_writel(0x01010000, DDR_CR087);	/* odt: wr_map_cs0
						 * rd_map_cs0
						 * port_data_err_id */
	__raw_writel(0x03030101, DDR_CR088);	/* todtl_2cmd_f1, _f0
						 * odt wr/rd_map_cs1 */
	__raw_writel(0x02020002, DDR_CR089);	/* add_odt stuff */

	__raw_writel(0x02020200, DDR_CR090);	/* w2r/r2wr2r _diffcs_dly */
	__raw_writel(0x00020101, DDR_CR091);
	__raw_writel(0x00000000, DDR_CR092);

	/* AXI ports */
	__raw_writel(0x00030000, DDR_CR117);	/* FIFO type (0-async, 1-2:1
						 *	2-1:2, 3- sync, w_pri
						 * r_pri
						 */
	__raw_writel(0x01010000, DDR_CR118);	/* w_pri, rpri, en */
	__raw_writel(0x00030003, DDR_CR119);	/* fifo_type */

	__raw_writel(0x02020000, DDR_CR120);
	__raw_writel(0x00000202, DDR_CR121);
	__raw_writel(0x01010064, DDR_CR122);
	__raw_writel(0x00000101, DDR_CR123);
	__raw_writel(0x00000064, DDR_CR124);

	/* TDFI */
	__raw_writel(0x00000000, DDR_CR125);
	__raw_writel(0x000a0b00, DDR_CR126);
	__raw_writel(0x0c280000, DDR_CR127);
	__raw_writel(0x02000200, DDR_CR128);
	__raw_writel(0x02000200, DDR_CR129);

	__raw_writel(0x00003cc8, DDR_CR131);
	__raw_writel(0x00000c30, DDR_CR131);
	__raw_writel(0x03c70404, DDR_CR132);	/* wrlat, rdlat */
	__raw_writel(0x02000200, DDR_CR133);
	__raw_writel(0x02000200, DDR_CR134);
	__raw_writel(0x000003c7, DDR_CR135);
	__raw_writel(0x000012e3, DDR_CR136);
	__raw_writel(0x02020404, DDR_CR137);	/* wrlat, rdlat */
	__raw_writel(0x80000301, DDR_CR138);

	__raw_writel(0x00000101, DDR_CR153);

        ddr_phy_init();

	__raw_writel(0x00000601, DDR_CR000);	/* LPDDR2 or DDR3, start */

	i = 200;
	while (1)
	{
		if (i == 0)
			break;
		i--;
	}
	return dram_size;
}

int dram_init(void)
{
#ifdef CONFIG_SKIP_RELOCATE_UBOOT
	gd->ram_size = CONFIG_SYS_INIT_RAM_SIZE;
#else
	/* dram_init must store complete ramsize in gd->ram_size */
//	gd->ram_size = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE,
//				PHYS_SDRAM_1_SIZE);

//	ddr_ctrl_init();
#endif
	gd->ram_size = 0x80000;
	return 0;
}

void setup_iomux_uart(void)
{
	// UART0
	__raw_writel(0x001021a3, IOMUXC_PAD_032);
	__raw_writel(0x001021a1, IOMUXC_PAD_033);
}

void setup_iomux_ddr(void)
{
	__raw_writel(0x000100a0, IOMUXC_DDR_CLK);
}

#if defined(CONFIG_CMD_NET)
int fecpin_setclear(struct eth_device *dev, int setclear)
{
	__raw_writel(0x00203191, IOMUXC_PAD_000);	// RMII_CLK

	__raw_writel(0x00103192, IOMUXC_PAD_045);	// MDC
	__raw_writel(0x00103193, IOMUXC_PAD_046);	// MDIO
	__raw_writel(0x00103191, IOMUXC_PAD_047);	// RxDV
	__raw_writel(0x00103191, IOMUXC_PAD_048);	// RxD1
	__raw_writel(0x00103191, IOMUXC_PAD_049);	// RxD0
	__raw_writel(0x00103191, IOMUXC_PAD_050);	// RxER
	__raw_writel(0x00103192, IOMUXC_PAD_051);	// TxD1
	__raw_writel(0x00103192, IOMUXC_PAD_052);	// TxD0
	__raw_writel(0x00103192, IOMUXC_PAD_053);	// TxEn
/*
	struct fec_info_s *info = (struct fec_info_s *)dev->priv;

	if (setclear) {
		if (info->iobase == 0x400D0000) {
			__raw_writel(0x00102982, IOMUXC_PAD_045);	// MDC
			__raw_writel(0x00102983, IOMUXC_PAD_046);	// MDIO
			__raw_writel(0x00102981, IOMUXC_PAD_047);	// RxDV
			__raw_writel(0x00102981, IOMUXC_PAD_048);	// RxD1
			__raw_writel(0x00102981, IOMUXC_PAD_049);	// RxD0
			__raw_writel(0x00102981, IOMUXC_PAD_050);	// RxER
			__raw_writel(0x00102982, IOMUXC_PAD_051);	// TxD1
			__raw_writel(0x00102982, IOMUXC_PAD_052);	// TxD0
			__raw_writel(0x00102982, IOMUXC_PAD_053);	// TxEn
		}

		if (info->iobase == CONFIG_SYS_FEC0_IOBASE) {
			__raw_writel(0x00102982, IOMUXC_PAD_054);	// MDC
			__raw_writel(0x00102983, IOMUXC_PAD_055);	// MDIO
			__raw_writel(0x00102981, IOMUXC_PAD_056);	// RxDV
			__raw_writel(0x00102981, IOMUXC_PAD_057);	// RxD1
			__raw_writel(0x00102981, IOMUXC_PAD_058);	// RxD0
			__raw_writel(0x00102981, IOMUXC_PAD_059);	// RxER
			__raw_writel(0x00102982, IOMUXC_PAD_060);	// TxD1
			__raw_writel(0x00102982, IOMUXC_PAD_061);	// TxD0
			__raw_writel(0x00102982, IOMUXC_PAD_062);	// TxEn
		}
	}
*/
	return 0;
}
#endif

void setup_iomux_fec(void)
{
	__raw_writel(0x00201181, IOMUXC_PAD_000);	// RMII_CLK

	__raw_writel(0x00101182, IOMUXC_PAD_045);	// MDC
	__raw_writel(0x00101183, IOMUXC_PAD_046);	// MDIO
	__raw_writel(0x00101181, IOMUXC_PAD_047);	// RxDV
	__raw_writel(0x00101181, IOMUXC_PAD_048);	// RxD1
	__raw_writel(0x00101181, IOMUXC_PAD_049);	// RxD0
	__raw_writel(0x00101181, IOMUXC_PAD_050);	// RxER
	__raw_writel(0x00101182, IOMUXC_PAD_051);	// TxD1
	__raw_writel(0x00101182, IOMUXC_PAD_052);	// TxD0
	__raw_writel(0x00101182, IOMUXC_PAD_053);	// TxEn
}

#ifdef CONFIG_MXC_SPI
void setup_iomux_spi(void)
{
}
#endif

#ifdef CONFIG_FSL_ESDHC
int board_mmc_getcd(struct mmc *mmc)
{
	//struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret;

	__raw_writel(0x005031e2, IOMUXC_PAD_045);	// clk
	__raw_writel(0x005031e2, IOMUXC_PAD_046);	// cmd
	__raw_writel(0x005031e3, IOMUXC_PAD_047);	// dat0
	__raw_writel(0x005031e3, IOMUXC_PAD_048);	// dat1
	__raw_writel(0x005031e3, IOMUXC_PAD_049);	// dat2
	__raw_writel(0x005031e3, IOMUXC_PAD_050);	// dat3

	__raw_writel(0x005031ef, IOMUXC_PAD_014);	// clk
	__raw_writel(0x005031ef, IOMUXC_PAD_015);	// cmd
	__raw_writel(0x005031ef, IOMUXC_PAD_016);	// dat0
	__raw_writel(0x005031ef, IOMUXC_PAD_017);	// dat1
	__raw_writel(0x005031ef, IOMUXC_PAD_018);	// dat2
	__raw_writel(0x005031ef, IOMUXC_PAD_019);	// dat3

	ret = 1;
	return ret;
}

int board_mmc_init(bd_t *bis)
{
	u32 index;
	s32 status = 0;
#if 0
	printf("%s: ", __FUNCTION__);
	for (index = 0; index < CONFIG_SYS_FSL_ESDHC_NUM;
			index++) {
		switch (index) {
		case 0:
			printf("sd %d\n", index);
			__raw_writel(0x005031e2, IOMUXC_PAD_045);	// clk
			__raw_writel(0x005031e2, IOMUXC_PAD_046);	// cmd
			__raw_writel(0x005031e3, IOMUXC_PAD_047);	// dat0
			__raw_writel(0x005031e3, IOMUXC_PAD_048);	// dat1
			__raw_writel(0x005031e3, IOMUXC_PAD_049);	// dat2
			__raw_writel(0x005031e3, IOMUXC_PAD_050);	// dat3
			__raw_writel(0x005031e2, IOMUXC_PAD_051);	// wp
			break;
		case 1:
			printf("sd %d\n", index);
			__raw_writel(0x005031a2, IOMUXC_PAD_014);	// clk
			__raw_writel(0x005031a2, IOMUXC_PAD_015);	// cmd
			__raw_writel(0x005031a3, IOMUXC_PAD_016);	// dat0
			__raw_writel(0x005031a3, IOMUXC_PAD_017);	// dat1
			__raw_writel(0x005031a3, IOMUXC_PAD_018);	// dat2
			__raw_writel(0x005031a3, IOMUXC_PAD_019);	// dat3
			__raw_writel(0x005031a2, IOMUXC_PAD_068);	// wp
			break;
		default:
			printf("Warning: you configured more ESDHC controller"
				"(%d) as supported by the board(2)\n",
				CONFIG_SYS_FSL_ESDHC_NUM);
			return status;
		}
	}
#endif
	status |= fsl_esdhc_initialize(bis, &esdhc_cfg[index]);
	return status;
}
#endif

int board_early_init_f(void)
{
	setup_iomux_uart();

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef CONFIG_MXC_SPI
	setup_iomux_spi();
#endif
	return 0;
}
#endif

int checkboard(void)
{
	puts("Board: VyBrid\n");

	return 0;
}
