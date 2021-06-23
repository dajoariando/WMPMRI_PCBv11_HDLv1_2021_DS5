/*
 * bstream.h
 *
 *  Created on: Jun 18, 2021
 *      Author: David Ariando
 */

#ifndef FUNCTIONS_BSTREAM_H_
#define FUNCTIONS_BSTREAM_H_

#define NORMAL 0
#define LOOP_STA 1
#define LOOP_STO 2

typedef struct bstream_objs {
	unsigned int * sram_addr;	// the sram address
	unsigned int curr_ofst;		// the sram current offset
	char loop_sta;				// the loop start flag
	char loop_sto;				// the loop stop flag
	char end_of_seq;			// the end of sequence flag
	unsigned long repetition;	// the number of repetition
	char error_seq;				// the error flag
} bstream_obj;

void bstream__push(bstream_obj * obj, char pattern_mode, char all_1s_mode, char all_0s_mode, char loop_sta, char loop_sto, char end_of_seq, unsigned long long dat_msb, unsigned long long dat_lsb);

void bstream__init(bstream_obj *obj, void *sram_addr, unsigned long repetition);

void bstream_rst();

void bstream_start();

char bstream_check(bstream_obj *obj);

void bstream__end_of_seq(bstream_obj *obj);

void bstream__all_1s(bstream_obj *obj, char mode, unsigned long len);

void bstream__all_0s(bstream_obj *obj, char mode, unsigned long len);

void bstream__pattern(bstream_obj *obj, char mode, unsigned long long dat_msb, unsigned long long dat_lsb);

#endif /* FUNCTIONS_BSTREAM_H_ */
