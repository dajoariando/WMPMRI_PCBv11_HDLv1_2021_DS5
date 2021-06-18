/*
 * bstream.h
 *
 *  Created on: Jun 18, 2021
 *      Author: David Ariando
 */

#ifndef FUNCTIONS_BSTREAM_H_
#define FUNCTIONS_BSTREAM_H_

typedef struct bstream_objs {
	unsigned int * sram_addr;
	unsigned int curr_ofst;
	char loop_sta;
	char loop_sto;
	char end_of_seq;
} bstream_obj;

void bstream__push(bstream_obj * obj, char pattern_mode, char all_1s_mode, char all_0s_mode, char loop_sta, char loop_sto, char end_of_seq, unsigned long long dat_msb, unsigned long long dat_lsb);

void useless(bstream_obj * obj);

void bstream__init(bstream_obj *obj, void *sram_addr, unsigned long repetition);

void bstream__loop_sta(bstream_obj *obj);

void bstream__loop_sto(bstream_obj *obj);   // loop stop has to be called before the functions

void bstream__end_of_seq(bstream_obj *obj);

void bstream__all_1s(bstream_obj *obj, unsigned long len);

void bstream__all_0s(bstream_obj *obj, unsigned long len);

void bstream__pattern(bstream_obj *obj, unsigned long long dat_msb, unsigned long long dat_lsb);

#endif /* FUNCTIONS_BSTREAM_H_ */
