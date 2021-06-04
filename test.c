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

int main() {

	double clk_freq = 12;

	init();

	/* set pll for CPMG
	 Set_PLL(lwaxi_sys_pll, 0, clk_freq, 0.5, DISABLE_MESSAGE);
	 Reset_PLL(lwaxi_cnt_out, SYS_PLL_RST_ofst, ctrl_out);
	 Set_DPS(lwaxi_sys_pll, 0, 0, DISABLE_MESSAGE);
	 Wait_PLL_To_Lock(lwaxi_cnt_in, PLL_SYS_lock_ofst);
	 */

	// reset the FIFO
	cnt_out |= BITSTR_FIFO_RST;
	*lwaxi_cnt_out = cnt_out;
	usleep(100);
	cnt_out &= ~BITSTR_FIFO_RST;
	*lwaxi_cnt_out = cnt_out;

	//
	int i;
	unsigned int x = 0x00000000;
	for (i = 0; i < 256; i++) {
		alt_write_word(axi_bitstr_fifo, (x++) & (~(1 << 31)));
	}
	alt_write_word(axi_bitstr_fifo, (1 << 31));

	// printf("data in fifo: %d\n", *lwaxi_bitstr_fifo_csr);

	// start the bitstream
	cnt_out |= BITSTR_START;
	*lwaxi_cnt_out = cnt_out;
	cnt_out &= ~BITSTR_START;
	*lwaxi_cnt_out = cnt_out;

	//for (i = 0; i < 128; i++) {
	//	alt_write_word(axi_bitstr_fifo, (x++) & (~(1 << 31)));
	//}
	//alt_write_word(axi_bitstr_fifo, (1 << 31));

	while (!(alt_read_word(lwaxi_cnt_in) & BITSTR_END))
		;

	// stop the bitstream
	cnt_out |= BITSTR_STOP;
	*lwaxi_cnt_out = cnt_out;
	cnt_out &= ~BITSTR_STOP;
	*lwaxi_cnt_out = cnt_out;

	// printf("data in fifo: %d\n", *lwaxi_bitstr_fifo_csr);

	leave();

	return 0;
}
