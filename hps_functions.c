// Created on: May 26, 2021
// Author: David Ariando

#include "hps_linux.h"
#include "glob_vars.h"

void open_physical_memory_device() {
	fd_dev_mem = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd_dev_mem == -1) {
		printf("ERROR: could not open \"/dev/mem\".\n");
		printf("    errno = %s\n", strerror(errno));
		exit (EXIT_FAILURE);
	}
}

void close_physical_memory_device() {
	close (fd_dev_mem);
}

void mmap_fpga_peripherals() {
	// IMPORTANT: If you try to only mmap the fpga leds, it is possible for the
	// operation to fail, and you will get "Invalid argument" as errno. The
	// mmap() manual page says that you can only map a file from an offset which
	// is a multiple of the system's page size.

	// In our specific case, our fpga leds are located at address 0xFF200000,
	// which is a multiple of the page size, however this is due to luck because
	// the fpga leds are the only peripheral connected to the h2f_lw_axi_master.
	// The typical page size in Linux is 0x1000 bytes.

	// So, generally speaking, you will have to mmap() the closest address which
	// is a multiple of your page size and access your peripheral by a specific
	// offset from the mapped address.

	lwaxi_base = mmap(NULL, h2f_lw_axi_master_span, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dev_mem, ALT_LWFPGASLVS_OFST);
	if (lwaxi_base == MAP_FAILED) {
		printf("Error: h2f_lw_axi_master mmap() failed.\n");
		printf("    errno = %s\n", strerror(errno));
		close (fd_dev_mem);
		exit (EXIT_FAILURE);
	}

	axi_base = mmap(NULL, HW_FPGA_AXI_SPAN, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dev_mem, ALT_AXI_FPGASLVS_OFST);
	if (axi_base == MAP_FAILED) {
		printf("Error: h2f_axi_master mmap() failed.\n");
		printf("    errno = %s\n", strerror(errno));
		close (fd_dev_mem);
		exit (EXIT_FAILURE);
	}

	// lwaxi master slave devices
	lwaxi_cnt_out = lwaxi_base + CNT_OUT_BASE;
	lwaxi_cnt_in = lwaxi_base + CNT_IN_BASE;
	lwaxi_sys_pll = lwaxi_base + SYS_PLL_RECONFIG_BASE;
	lwaxi_bitstr_fifo_csr = lwaxi_base + BITSTR_FIFO_IN_CSR_BASE;
	lwaxi_led = lwaxi_base + LED_PIO_BASE;
	lwaxi_sw = lwaxi_base + DIPSW_PIO_BASE;
	lwaxi_button = lwaxi_base + BUTTON_PIO_BASE;

	// axi master slave devices
	axi_bitstr_fifo = axi_base + BITSTR_FIFO_IN_BASE;
	axi_ram_tx_h1 = axi_base + TX_H1_BASE;

}

void munmap_fpga_peripherals() {

	if (munmap(lwaxi_base, h2f_lw_axi_master_span) != 0) {
		printf("Error: h2f_lw_axi_master munmap() failed\n");
		printf("    errno = %s\n", strerror(errno));
		close (fd_dev_mem);
		exit (EXIT_FAILURE);
	}

	lwaxi_base = NULL;

}

void mmap_peripherals() {
	mmap_fpga_peripherals();
}

void munmap_peripherals() {
	munmap_fpga_peripherals();
}

void init_default_system_param() {

// initialize control lines to default value
	cnt_out = CNT_OUT_default;
	alt_write_word((lwaxi_cnt_out), cnt_out);
	usleep(100);

// set reconfig configuration for pll's
	Reconfig_Mode(lwaxi_sys_pll, 1);   // polling mode for main pll

}

void init() {
	open_physical_memory_device();
	mmap_peripherals();
	init_default_system_param();
}

void leave() {
	munmap_peripherals();
	close_physical_memory_device();
}
