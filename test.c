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
unsigned int *lwaxi_cnt_out = NULL;			// control output to FPGA
unsigned int *lwaxi_cnt_in = NULL;			// control input from FPGA
unsigned int *lwaxi_led = NULL;					// LED
unsigned int *lwaxi_sw = NULL;					// switches
unsigned int *lwaxi_button = NULL;				// button
// memory map peripherals for bitstream codes. Also connect the bitstream object and ram in function bstream__init_all_sram() inside bstream.c
unsigned int *axi_ram_tx_h1 = NULL;
unsigned int *axi_ram_tx_l1 = NULL;
unsigned int *axi_ram_tx_aux = NULL;
unsigned int *axi_ram_tx_h2 = NULL;
unsigned int *axi_ram_tx_l2 = NULL;
unsigned int *axi_ram_tx_charge = NULL;
unsigned int *axi_ram_tx_damp = NULL;
unsigned int *axi_ram_tx_dump = NULL;

// bitstream objects
bstream_obj bstream_objs[BSTREAM_COUNT];

int main(int argc, char * argv[]) {

	double dtcl = atof(argv[1]);
	double ind_pchg_us = atof(argv[2]);
	double dump_len_us = atof(argv[3]);
	unsigned int en_pchrg = atoi(argv[4]);
	unsigned int repetition = atoi(argv[5]);
	float RFCLK = atof(argv[6]);

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

	// test precharging and dump
	// double pchg_us = 1000;
	// bstream__prechrg_n_dump(CLK_50, pchg_us, plen_us, tail_us, dump_dly_us, dump_len_us, en_pchrg);

	// test nulling
	// bstream__null_everything();

	// test rf output
	bstream__prechrg_n_rf_n_dump(CLK_50, RFCLK, dtcl, ind_pchg_us, dump_len_us, en_pchrg, repetition);

	leave();

	return 0;
}
