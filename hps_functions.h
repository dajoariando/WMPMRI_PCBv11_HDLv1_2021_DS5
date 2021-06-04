/*
 * hps_functions.h
 *
 *  Created on: May 27, 2021
 *      Author: David Ariando
 */

#ifndef HPS_FUNCTIONS_H_
#define HPS_FUNCTIONS_H_

void open_physical_memory_device();
void close_physical_memory_device();
void munmap_fpga_peripherals();
void mmap_peripherals();
void munmap_peripherals();
void init_default_system_param();
void init();
void leave();

#endif /* HPS_FUNCTIONS_H_ */
