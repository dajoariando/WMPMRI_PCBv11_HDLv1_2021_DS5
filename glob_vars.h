/*
 * glob_vars.h
 *
 *  Created on: May 27, 2021
 *      Author: David Ariando
 */

#ifndef GLOB_VARS_H_
#define GLOB_VARS_H_

// global variables
extern int fd_dev_mem;   // physical memory file descriptor
extern unsigned int cnt_out;

// memory mapped peripherals
extern void *lwaxi_base;					// the LWAXI bus mm base address
extern void *axi_base;					// the AXI bus mm base address
extern void *lwaxi_sys_pll;						// system pll reconfig
extern unsigned int *lwaxi_cnt_out;			// control output to FPGA
extern unsigned int *lwaxi_cnt_in;			// control input from FPGA
extern unsigned int *axi_bitstr_fifo;	// bitstream fifo data address
extern unsigned int *lwaxi_bitstr_fifo_csr;   // bitstream fifo status address
extern unsigned int *lwaxi_led;					// LED
extern unsigned int *lwaxi_sw;					// switches
extern unsigned int *lwaxi_button;				// button

// additional parameters
size_t h2f_lw_axi_master_span = ALT_LWFPGASLVS_UB_ADDR - ALT_LWFPGASLVS_LB_ADDR + 1;
#define ALT_AXI_FPGASLVS_OFST (0xC0000000) // axi_master
#define HW_FPGA_AXI_SPAN (0x40000000) // Bridge span
#define HW_FPGA_AXI_MASK ( HW_FPGA_AXI_SPAN - 1 )

#endif /* GLOB_VARS_H_ */
