// Created on: May 26, 2021
// Author: David Ariando

#include "hps_linux.h"

// global variables
int fd_dev_mem = 0;   // physical memory file descriptor
unsigned int cnt_out = 0;

// memory mapped peripherals. Also add extern definition at glob_vars.h
void *lwaxi_base = NULL;					// the LWAXI bus mm base address
void *axi_base = NULL;					// the AXI bus mm base address
void *lwaxi_sys_pll = NULL;						// system pll reconfig
volatile unsigned int *lwaxi_cnt_out = NULL;		// control output to FPGA
volatile unsigned int *lwaxi_cnt_in = NULL;			// control input from FPGA
volatile unsigned int *lwaxi_led = NULL;					// LED
volatile unsigned int *lwaxi_sw = NULL;					// switches
volatile unsigned int *lwaxi_button = NULL;				// button
volatile unsigned int *lwaxi_rx_dac = NULL;			// RX DAC for coil tuning

// memory map peripherals for bitstream codes. Also connect the bitstream object and ram in function bstream__init_all_sram() inside bstream.c
volatile unsigned int *axi_ram_tx_h1 = NULL;
volatile unsigned int *axi_ram_tx_l1 = NULL;
volatile unsigned int *axi_ram_tx_aux = NULL;
volatile unsigned int *axi_ram_tx_h2 = NULL;
volatile unsigned int *axi_ram_tx_l2 = NULL;
volatile unsigned int *axi_ram_tx_charge = NULL;
volatile unsigned int *axi_ram_tx_damp = NULL;
volatile unsigned int *axi_ram_tx_dump = NULL;
volatile unsigned int *axi_ram_rx_inc_damp = NULL;
volatile unsigned int *axi_ram_rx_in_short = NULL;

// bitstream objects
bstream_obj bstream_objs[BSTREAM_COUNT];

int main(int argc, char * argv[]) {

	char RF_mode = atoi(argv[1]);
	double dtcl = atof(argv[2]);
	double ind_pchg_us = atof(argv[3]);
	double plen_us = atof(argv[4]);
	double tail_us = atof(argv[5]);
	double dump_dly_us = atof(argv[6]);
	double dump_len_us = atof(argv[7]);
	unsigned int en_pchrg = atoi(argv[8]);
	unsigned int repetition = atoi(argv[9]);
	float RFCLK = atof(argv[10]);
	float vvarac = atof(argv[11]);
	double tx_coil_pchg_us = atof(argv[12]);
	unsigned int dump_repetition = atoi(argv[13]);

	double max_plen = 500;   // set the maximum plen
	if (ind_pchg_us > max_plen) {
		printf("\t ERROR! Pulse is too long.\n");
		return 0;
	}

	init();

	/*
	 *  set pll for CPMG
	 *
	 *  double clk_freq = 12;
	 Set_PLL(lwaxi_sys_pll, 0, clk_freq, 0.5, DISABLE_MESSAGE);
	 Reset_PLL(lwaxi_cnt_out, SYS_PLL_RST_ofst, ctrl_out);
	 Set_DPS(lwaxi_sys_pll, 0, 0, DISABLE_MESSAGE);
	 Wait_PLL_To_Lock(lwaxi_cnt_in, PLL_SYS_lock_ofst);
	 */

	// alt_write_word(lwaxi_led, 0xFF);
	// alt_write_word(lwaxi_led, 0x00);
	// standard parameters
	float CLK_50 = 50.00;
	//float RFCLK = 1.00;

	// reset
	bstream_rst();

	// test nulling
	bstream__null_everything();

	// test precharging and dump
	// double bstrap_pchg_us = 1000;
	// bstream__prechrg_n_dump(CLK_50, bstrap_pchg_us, ind_pchg_us, tail_us, dump_dly_us, dump_len_us, en_pchrg);

	// test rf output
	// bstream__prechrg_n_rf_n_dump(RF_mode, CLK_50, RFCLK, dtcl, ind_pchg_us, tx_coil_pchg_us, dump_len_us, en_pchrg, repetition);
	bstream__prechrg_n_rf_n_dump_180(/*RF_mode,*/CLK_50, RFCLK, dtcl, ind_pchg_us, tx_coil_pchg_us, dump_len_us, en_pchrg, repetition, dump_repetition);

	// slow toggle output
	//bstream__toggle(&bstream_objs[tx_l1], CLK_50, 3000000, 1000);
	//bstream__toggle(&bstream_objs[tx_l2], CLK_50, 3000000, 1000);
	//bstream__toggle(&bstream_objs[tx_h1], CLK_50, 3000000, 1000);
	//bstream__toggle(&bstream_objs[tx_h2], CLK_50, 3000000, 1000);

	// init_dac_ad5722r(lwaxi_rx_dac);
	// wr_dac_ad5722r(lwaxi_rx_dac, DAC_B, vvarac, ENABLE_MESSAGE);

	leave();

	return 0;
}
