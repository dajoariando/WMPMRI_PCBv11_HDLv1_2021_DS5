/*
 * bstream.c
 *
 *  Created on: Jun 18, 2021
 *      Author: David Ariando
 */

#include "bstream.h"
#include "../hps_linux.h"
#include "../glob_vars.h"

void bstream__push(bstream_obj * obj, char pattern_mode, char all_1s_mode, char all_0s_mode, char loop_sta, char loop_sto, char end_of_seq, unsigned long long dat_msb, unsigned long long dat_lsb) {

	alt_write_word(obj->sram_addr + obj->curr_ofst + 0, (unsigned int) (dat_lsb & 0xFFFFFFFF));
	alt_write_word(obj->sram_addr + obj->curr_ofst + 1, (unsigned int) (dat_lsb >> 32 & 0xFFFFFFFF));
	alt_write_word(obj->sram_addr + obj->curr_ofst + 2, (unsigned int) (dat_msb & 0xFFFFFFFF));
	alt_write_word(obj->sram_addr + obj->curr_ofst + 3, (unsigned int) (dat_msb >> 32 & 0x0FFFFFFF) | (end_of_seq & 0x01) << 31 | (unsigned int) (loop_sta & 0x01) << 30 | (unsigned int) (loop_sto & 0x01) << 29 | (unsigned int) (pattern_mode & 0x01) << 28 | (unsigned int) (all_1s_mode & 0x01) << 27 | (unsigned int) (all_0s_mode & 0x01) << 26);

	// increment addresses
	obj->curr_ofst += 4;

	// reset all looping variables after pushing the data
	obj->loop_sta = 0;
	obj->loop_sto = 0;
	obj->end_of_seq = 0;

	return;
}

void bstream__init(bstream_obj *obj, void *sram_addr, unsigned long repetition) {

	obj->sram_addr = sram_addr;
	obj->curr_ofst = 0;
	obj->loop_sta = 0;
	obj->loop_sto = 0;
	obj->end_of_seq = 0;
	obj->repetition = repetition;
	obj->error_seq = SEQ_OK;   // set the error sequence to be 0

	// write the repetition and increment the address by 4
	alt_write_word(obj->sram_addr + obj->curr_ofst + 0, (unsigned int) (repetition & 0xFFFFFFFF));
	obj->curr_ofst += 4;
}

void bstream_rst() {

	// reset
	cnt_out |= BITSTR_ADV_RST;
	*lwaxi_cnt_out = cnt_out;
	cnt_out &= ~BITSTR_ADV_RST;
	*lwaxi_cnt_out = cnt_out;

}

void bstream_start() {
	// start
	cnt_out |= BITSTR_ADV_START;
	*lwaxi_cnt_out = cnt_out;
	cnt_out &= ~BITSTR_ADV_START;
	*lwaxi_cnt_out = cnt_out;
}

char bstream_check(bstream_obj *obj) {
	if (obj->error_seq == SEQ_ERROR) {
		return SEQ_ERROR;
	} else
		return SEQ_OK;
}

void bstream__end_of_seq(bstream_obj *obj) {
	obj->loop_sta = 0;
	obj->loop_sto = 0;
	obj->end_of_seq = 1;

	bstream__push(obj, 0 /*pattern*/, 0 /* all 1s*/, 0 /* all 0s*/, obj->loop_sta, obj->loop_sto, obj->end_of_seq, 0, 0);
}

void bstream__all_1s(bstream_obj *obj, char mode, unsigned long len) {
	if (mode == LOOP_STA) {
		obj->loop_sta = 1;
		obj->loop_sto = 0;
		obj->end_of_seq = 0;
	}
	if (mode == LOOP_STO) {
		obj->loop_sta = 0;
		obj->loop_sto = 1;
		obj->end_of_seq = 0;
	}
	if (mode == NORMAL) {
		obj->loop_sta = 0;
		obj->loop_sto = 0;
		obj->end_of_seq = 0;
	}

	if (len < 10) {
		printf("\tERROR! The bitstream all-1s length should be at least 10.\n");
		obj->error_seq = 1;
	} else {
		bstream__push(obj, 0 /*pattern*/, 1 /* all 1s*/, 0 /* all 0s*/, obj->loop_sta, obj->loop_sto, obj->end_of_seq, 0, (unsigned long long) len);
	}
}

void bstream__all_0s(bstream_obj *obj, char mode, unsigned long len) {
	if (mode == LOOP_STA) {
		obj->loop_sta = 1;
		obj->loop_sto = 0;
		obj->end_of_seq = 0;
	}
	if (mode == LOOP_STO) {
		obj->loop_sta = 0;
		obj->loop_sto = 1;
		obj->end_of_seq = 0;
	}
	if (mode == NORMAL) {
		obj->loop_sta = 0;
		obj->loop_sto = 0;
		obj->end_of_seq = 0;
	}

	if (len < 10) {
		printf("\tERROR! The bitstream all-0s length should be at least 10.\n");
		obj->error_seq = 1;
	} else {
		bstream__push(obj, 0 /*pattern*/, 0 /* all 1s*/, 1 /* all 0s*/, obj->loop_sta, obj->loop_sto, obj->end_of_seq, 0, (unsigned long long) len);
	}
}

void bstream__pattern(bstream_obj *obj, char mode, unsigned long long dat_msb, unsigned long long dat_lsb) {
	if (mode == LOOP_STA) {
		obj->loop_sta = 1;
		obj->loop_sto = 0;
		obj->end_of_seq = 0;
	}
	if (mode == LOOP_STO) {
		obj->loop_sta = 0;
		obj->loop_sto = 1;
		obj->end_of_seq = 0;
	}
	if (mode == NORMAL) {
		obj->loop_sta = 0;
		obj->loop_sto = 0;
		obj->end_of_seq = 0;
	}

	bstream__push(obj, 1 /*pattern*/, 0 /* all 1s*/, 0 /* all 0s*/, obj->loop_sta, obj->loop_sto, obj->end_of_seq, dat_msb, dat_lsb);
}

