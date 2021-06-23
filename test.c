// Created on: May 26, 2021
// Author: David Ariando

#include "hps_linux.h"

// global variables
int fd_dev_mem = 0;   // physical memory file descriptor
unsigned int cnt_out = 0;

// memory mapped peripherals
void *lwaxi_base = NULL;					// the LWAXI bus mm base address
void *axi_base = NULL;					// the AXI bus mm base address
void *lwaxi_sys_pll = NULL;						// system pll reconfig
unsigned int *lwaxi_cnt_out = NULL;			// control output to FPGA
unsigned int *lwaxi_cnt_in = NULL;			// control input from FPGA
unsigned int *axi_bitstr_fifo = NULL;	// bitstream fifo data address
unsigned int *lwaxi_bitstr_fifo_csr = NULL;   // bitstream fifo status address
unsigned int *lwaxi_led = NULL;					// LED
unsigned int *lwaxi_sw = NULL;					// switches
unsigned int *lwaxi_button = NULL;				// button
unsigned int *axi_ram_tx_h1 = NULL;

int main() {

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
	// reset
	bstream_rst();

	bstream_obj obj_gpio0_1;		// create the object (connected to the GPIO)

	bstream__init(&obj_gpio0_1, axi_ram_tx_h1, 0x03);   // initialize the object along with the SRAM address

	bstream__all_1s(&obj_gpio0_1, NORMAL, 10);		// generate 10 cycles of 1s
	bstream__all_0s(&obj_gpio0_1, NORMAL, 10);		// generate 10 cycles of 0s
	bstream__all_1s(&obj_gpio0_1, NORMAL, 30);		// generate 30 cycles of 1s

	bstream__pattern(&obj_gpio0_1, NORMAL, 0x0AA0, 0x0000);   // set the pattern 0xAAAA (120-bit)
	bstream__pattern(&obj_gpio0_1, LOOP_STA, 0x0F00, 0x0000);
	bstream__pattern(&obj_gpio0_1, NORMAL, 0x0FF0, 0x0000);   // set the pattern 0xF0F0 (120-bit)
	bstream__pattern(&obj_gpio0_1, LOOP_STO, 0xAAAA, 0x0000);   // set the pattern 0xF0F0 (120-bit)

	bstream__end_of_seq(&obj_gpio0_1);				// set end of the sequence

	if (bstream_check(&obj_gpio0_1) == SEQ_OK) {
		bstream_start();
	} else {
		printf("\tERROR! Bitstream sequence has an error.\n");
	}

	leave();

	return 0;
}
