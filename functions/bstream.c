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

	alt_write_word(obj->sram_addr + obj->curr_ofst + 0, (unsigned int) ( dat_lsb & 0xFFFFFFFF ));
	alt_write_word(obj->sram_addr + obj->curr_ofst + 1, (unsigned int) ( dat_lsb >> 32 & 0xFFFFFFFF ));
	alt_write_word(obj->sram_addr + obj->curr_ofst + 2, (unsigned int) ( dat_msb & 0xFFFFFFFF ));
	alt_write_word(obj->sram_addr + obj->curr_ofst + 3, (unsigned int) ( dat_msb >> 32 & 0x0FFFFFFF ) | ( end_of_seq & 0x01 ) << 31 | (unsigned int) ( loop_sta & 0x01 ) << 30 | (unsigned int) ( loop_sto & 0x01 ) << 29 | (unsigned int) ( pattern_mode & 0x01 ) << 28 | (unsigned int) ( all_1s_mode & 0x01 ) << 27 | (unsigned int) ( all_0s_mode & 0x01 ) << 26);

	// increment addresses
	obj->curr_ofst += 4;

	// reset all looping variables after pushing the data
	obj->loop_sta = 0;
	obj->loop_sto = 0;
	obj->end_of_seq = 0;

	return;
}

void bstream__init_all_sram() {

	bstream_objs[tx_h1].sram_addr = axi_ram_tx_h1;
	bstream_objs[tx_l1].sram_addr = axi_ram_tx_l1;
	bstream_objs[tx_h2].sram_addr = axi_ram_tx_h2;
	bstream_objs[tx_l2].sram_addr = axi_ram_tx_l2;
	bstream_objs[tx_damp].sram_addr = axi_ram_tx_damp;
	bstream_objs[tx_dump].sram_addr = axi_ram_tx_dump;
	bstream_objs[tx_charge].sram_addr = axi_ram_tx_charge;
	bstream_objs[tx_aux].sram_addr = axi_ram_tx_aux;
	bstream_objs[rx_inc_damp].sram_addr = axi_ram_rx_inc_damp;
	bstream_objs[rx_in_short].sram_addr = axi_ram_rx_in_short;

}

void bstream__init(bstream_obj *obj, unsigned long repetition, float freq_MHz) {

	obj->curr_ofst = 0;
	obj->loop_sta = 0;
	obj->loop_sto = 0;
	obj->end_of_seq = 0;
	obj->repetition = repetition;
	obj->freq_MHz = freq_MHz;
	obj->error_seq = SEQ_OK;   // set the error sequence to be 0

	// check if sram_addr is pre-initialized, otherwise generate error
	if (obj->sram_addr == 0) {
		obj->error_seq = SEQ_ERROR;
		printf("\tERROR! Bitstream SRAM address is not initialized! Add lines in bstream__init_all_sram() for all bitstream devices!\n");
	}
	if (obj->repetition < 1) {
		printf("\tWARNING! Loop number cannot be less than 1!\n");
	}

	// write the repetition and increment the address by 4
	alt_write_word(obj->sram_addr + obj->curr_ofst + 0, (unsigned int) ( repetition & 0xFFFFFFFF ));
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
	unsigned char ii;
	char stop = SEQ_OK;   // do not start the sequence if there's any error
	for (ii = 0; ii < BSTREAM_COUNT; ii++) {
		stop = bstream_check(&bstream_objs[ii]);
		if (stop == SEQ_ERROR) {
			printf("\tERROR! Bitstream sequence has an error.\n");
			return;
		};
	}

	cnt_out |= BITSTR_ADV_START;
	*lwaxi_cnt_out = cnt_out;
	cnt_out &= ~BITSTR_ADV_START;
	*lwaxi_cnt_out = cnt_out;

}

char bstream_check(bstream_obj *obj) {
	if (obj->error_seq == SEQ_ERROR) {
		return SEQ_ERROR;
	}
	else
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
		obj->error_seq = SEQ_ERROR;
	}
	else {
		bstream__push(obj, 0 /*pattern*/, 1 /* all 1s*/, 0 /* all 0s*/, obj->loop_sta, obj->loop_sto, obj->end_of_seq, 0, (unsigned long long) len);
	}
}

void bstream__all_1s_us(bstream_obj *obj, char mode, float len_us) {
	bstream__all_1s(obj, mode, (unsigned int) ( ( round(len_us * ( obj->freq_MHz )) ) + 0.1 ));
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
		obj->error_seq = SEQ_ERROR;
	}
	else {
		bstream__push(obj, 0 /*pattern*/, 0 /* all 1s*/, 1 /* all 0s*/, obj->loop_sta, obj->loop_sto, obj->end_of_seq, 0, (unsigned long long) len);
	}
}

void bstream__all_0s_us(bstream_obj *obj, char mode, float len_us) {
	bstream__all_0s(obj, mode, (unsigned int) ( ( round(len_us * ( obj->freq_MHz )) ) + 0.1 ));
}

void bstream__pattern(bstream_obj *obj, char mode, unsigned long long dat_msb, unsigned long long dat_lsb) {
	if ( ( dat_msb >> ( PATTERN_WIDTH - 64 ) ) > 0) {   // check if the pattern given uses more than 120-bit
		printf("\tERROR! Pattern is more than %d bits.\n", PATTERN_WIDTH);
		obj->error_seq = SEQ_ERROR;
	}

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

// special functions

void bstream__null_output(bstream_obj *obj) {
	bstream__init(obj, 5, 10);   // set the sram address, with number of repetition 5, and 10 MHz input frequency. Numbers are not important.
	bstream__all_0s(obj, NORMAL, 10);   // set 0s for 10 cycles. Number is not important.
	bstream__end_of_seq(obj);
}

void bstream__toggle(bstream_obj *obj, float freq_MHz, float pulse_us, unsigned long repetition) {
	bstream__init(obj, repetition, freq_MHz);
	bstream__all_1s_us(obj, LOOP_STA, pulse_us);
	bstream__all_0s_us(obj, LOOP_STO, pulse_us);
	bstream__end_of_seq(obj);

	bstream_start();
}

void bstream__null_everything() {
	unsigned char ii;
	for (ii = 0; ii < BSTREAM_COUNT; ii++) {
		bstream__null_output (&bstream_objs[ii]);
	}
}

void bstream__prechrg_n_dump(float clk_MHz, double bstrap_pchg_us, double ind_pchg_us, double tail_us, double dump_dly_us, double dump_len_us, unsigned char en_pchrg) {
	// GATE CONTROLS
	bstream__init(&bstream_objs[tx_h1], 10, clk_MHz);   // initialize the object along with the SRAM address
	bstream__all_0s_us(&bstream_objs[tx_h1], NORMAL, bstrap_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_h1], NORMAL, ind_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_h1], NORMAL, tail_us);
	bstream__end_of_seq (&bstream_objs[tx_h1]);   // set end of the sequence

	bstream__init(&bstream_objs[tx_h2], 10, clk_MHz);
	bstream__all_0s_us(&bstream_objs[tx_h2], NORMAL, bstrap_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_h2], NORMAL, ind_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_h2], NORMAL, tail_us);
	bstream__end_of_seq(&bstream_objs[tx_h2]);

	bstream__init(&bstream_objs[tx_l1], 10, clk_MHz);   // initialize the object along with the SRAM address
	bstream__all_1s_us(&bstream_objs[tx_l1], NORMAL, bstrap_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l1], NORMAL, ind_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l1], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_l1], NORMAL, tail_us);
	bstream__end_of_seq(&bstream_objs[tx_l1]);   // set end of the sequence

	bstream__init(&bstream_objs[tx_l2], 10, clk_MHz);
	bstream__all_1s_us(&bstream_objs[tx_l2], NORMAL, bstrap_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l2], NORMAL, ind_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l2], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_l2], NORMAL, tail_us);
	bstream__end_of_seq(&bstream_objs[tx_l2]);

	// OTHER CONTROLS
	bstream__init(&bstream_objs[tx_damp], 0x01, clk_MHz);
	bstream__all_0s_us(&bstream_objs[tx_damp], NORMAL, bstrap_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_damp], NORMAL, ind_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_damp], NORMAL, tail_us);
	bstream__end_of_seq(&bstream_objs[tx_damp]);

	bstream__init(&bstream_objs[tx_dump], 0x01, clk_MHz);
	bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, bstrap_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, ind_pchg_us - 2);
	bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, dump_dly_us + 2);
	bstream__all_1s_us(&bstream_objs[tx_dump], NORMAL, dump_len_us);
	bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, tail_us);
	bstream__end_of_seq(&bstream_objs[tx_dump]);

	bstream__init(&bstream_objs[tx_charge], 0x01, clk_MHz);
	bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, bstrap_pchg_us);
	if (en_pchrg) {
		bstream__all_1s_us(&bstream_objs[tx_charge], NORMAL, ind_pchg_us);
	}
	else {
		bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, ind_pchg_us);
	}
	bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, tail_us);
	bstream__end_of_seq(&bstream_objs[tx_charge]);

	// AUX
	bstream__init(&bstream_objs[tx_aux], 0x01, clk_MHz);
	bstream__all_1s(&bstream_objs[tx_aux], NORMAL, 10);   // generate 10 cycles of 1s
	bstream__all_0s(&bstream_objs[tx_aux], NORMAL, 10);   // generate 10 cycles of 0s
	bstream__end_of_seq(&bstream_objs[tx_aux]);

	bstream_start();
}

void bstream__prechrg_n_rf_n_dump(char mode, float clk_MHz, float RFclk_MHz, double dtcl, double ind_pchg_us, double tx_coil_pchg_us, double dump_len_us, unsigned char en_pchrg, unsigned int repetition) {

	// RF output mode (char mode):
	// 0x01: same duty cycle for Hi1/Lo2 (the same goes for Hi2/Lo1 pair)
	// 0x02: Hi1 duty cycle is higher than Lo2 (the same goes for Hi2/Lo1 pair)
	// 0x03: Hi1 duty cycle is lower than Lo2 (the same goes for Hi2/Lo1 pair)
	// char mode = 0x03;

	// init all the bitstream
	bstream__init(&bstream_objs[tx_aux], repetition, clk_MHz);
	bstream__init(&bstream_objs[tx_h1], repetition, clk_MHz);
	bstream__init(&bstream_objs[tx_h2], repetition, clk_MHz);
	bstream__init(&bstream_objs[tx_l1], repetition, clk_MHz);
	bstream__init(&bstream_objs[tx_l2], repetition, clk_MHz);
	bstream__init(&bstream_objs[tx_damp], repetition, clk_MHz);
	bstream__init(&bstream_objs[tx_dump], repetition, clk_MHz);
	bstream__init(&bstream_objs[tx_charge], repetition, clk_MHz);

	// precharge the bootstrap
	double bstrap_pchg_us = 1000;   // precharge the bootstrap for 1ms
	bstream__all_1s_us(&bstream_objs[tx_aux], NORMAL, bstrap_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_h1], NORMAL, bstrap_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_h2], NORMAL, bstrap_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l1], NORMAL, bstrap_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l2], NORMAL, bstrap_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_damp], NORMAL, bstrap_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, bstrap_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, bstrap_pchg_us);

	// precharge the inductor
	bstream__all_0s_us(&bstream_objs[tx_aux], NORMAL, ind_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_h1], NORMAL, ind_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_h2], NORMAL, ind_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l1], NORMAL, ind_pchg_us);
	bstream__all_1s_us(&bstream_objs[tx_l2], NORMAL, ind_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_damp], NORMAL, ind_pchg_us);
	bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, ind_pchg_us);
	if (en_pchrg) {
		bstream__all_1s_us(&bstream_objs[tx_charge], NORMAL, ind_pchg_us);
	}
	else {
		bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, ind_pchg_us);
	}

	// generate RF pulse
	unsigned int RFclk_period = (unsigned int) round(clk_MHz / RFclk_MHz);
	unsigned int RF_Hi = (unsigned int) round(dtcl * clk_MHz / RFclk_MHz);
	unsigned int RF_Lo = RFclk_period - RF_Hi;

	// extend the precharge phase to buy some time to get phase shift
	unsigned long dly_twk = 100;
	bstream__all_1s(&bstream_objs[tx_aux], NORMAL, dly_twk);
	if (mode == 0x01) {
		bstream__all_1s(&bstream_objs[tx_h1], NORMAL, dly_twk - ( ( RF_Hi - RF_Lo ) >> 1 ));
		bstream__all_1s(&bstream_objs[tx_h2], NORMAL, dly_twk);
		bstream__all_1s(&bstream_objs[tx_l1], NORMAL, dly_twk);
		bstream__all_1s(&bstream_objs[tx_l2], NORMAL, dly_twk - ( ( RF_Hi - RF_Lo ) >> 1 ));
	}
	else if (mode == 0x02) {
		bstream__all_1s(&bstream_objs[tx_h1], NORMAL, dly_twk - ( ( RF_Hi - RF_Lo ) >> 1 ) - ( ( RF_Hi - RF_Lo ) >> 3 ));
		bstream__all_1s(&bstream_objs[tx_h2], NORMAL, dly_twk + ( ( RF_Hi - RF_Lo ) >> 3 ));
		bstream__all_1s(&bstream_objs[tx_l1], NORMAL, dly_twk);
		bstream__all_1s(&bstream_objs[tx_l2], NORMAL, dly_twk - ( ( RF_Hi - RF_Lo ) >> 1 ));
	}
	else if (mode == 0x03) {
		bstream__all_1s(&bstream_objs[tx_h1], NORMAL, dly_twk - ( ( RF_Hi - RF_Lo ) >> 1 ));
		bstream__all_1s(&bstream_objs[tx_h2], NORMAL, dly_twk);
		bstream__all_1s(&bstream_objs[tx_l1], NORMAL, dly_twk + ( ( RF_Hi - RF_Lo ) >> 3 ));
		bstream__all_1s(&bstream_objs[tx_l2], NORMAL, dly_twk - ( ( RF_Hi - RF_Lo ) >> 1 ) - ( ( RF_Hi - RF_Lo ) >> 3 ));
	}
	bstream__all_0s(&bstream_objs[tx_damp], NORMAL, dly_twk);
	bstream__all_0s(&bstream_objs[tx_dump], NORMAL, dly_twk);
	if (en_pchrg) {
		bstream__all_1s(&bstream_objs[tx_charge], NORMAL, dly_twk);
	}
	else {
		bstream__all_0s(&bstream_objs[tx_charge], NORMAL, dly_twk);
	}

	/* precharge the tx coil
	 bstream__all_1s_us(&bstream_objs[tx_h1], NORMAL, tx_coil_pchg_us);
	 bstream__all_0s_us(&bstream_objs[tx_h2], NORMAL, tx_coil_pchg_us);
	 bstream__all_0s_us(&bstream_objs[tx_l1], NORMAL, tx_coil_pchg_us);
	 bstream__all_1s_us(&bstream_objs[tx_l2], NORMAL, tx_coil_pchg_us);
	 bstream__all_0s_us(&bstream_objs[tx_damp], NORMAL, tx_coil_pchg_us);
	 bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, tx_coil_pchg_us);
	 if (en_pchrg) {
	 bstream__all_1s_us(&bstream_objs[tx_charge], NORMAL, tx_coil_pchg_us);
	 }
	 else {
	 bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, tx_coil_pchg_us);
	 }
	 */

	bstream__all_0s(&bstream_objs[tx_aux], LOOP_STA, RF_Hi);
	if (mode == 0x01) {
		bstream__all_1s(&bstream_objs[tx_h1], LOOP_STA, RF_Hi);
		bstream__all_0s(&bstream_objs[tx_h2], LOOP_STA, RF_Lo);
		bstream__all_0s(&bstream_objs[tx_l1], LOOP_STA, RF_Lo);
		bstream__all_1s(&bstream_objs[tx_l2], LOOP_STA, RF_Hi);
	}
	else if (mode == 0x02) {
		bstream__all_1s(&bstream_objs[tx_h1], LOOP_STA, RF_Hi + ( ( RF_Hi - RF_Lo ) >> 2 ));
		bstream__all_0s(&bstream_objs[tx_h2], LOOP_STA, RF_Lo - ( ( RF_Hi - RF_Lo ) >> 2 ));
		bstream__all_0s(&bstream_objs[tx_l1], LOOP_STA, RF_Lo);
		bstream__all_1s(&bstream_objs[tx_l2], LOOP_STA, RF_Hi);
	}
	else if (mode == 0x03) {
		bstream__all_1s(&bstream_objs[tx_h1], LOOP_STA, RF_Hi);
		bstream__all_0s(&bstream_objs[tx_h2], LOOP_STA, RF_Lo);
		bstream__all_0s(&bstream_objs[tx_l1], LOOP_STA, RF_Lo - ( ( RF_Hi - RF_Lo ) >> 2 ));
		bstream__all_1s(&bstream_objs[tx_l2], LOOP_STA, RF_Hi + ( ( RF_Hi - RF_Lo ) >> 2 ));
	}
	bstream__all_0s(&bstream_objs[tx_damp], LOOP_STA, RF_Hi);
	bstream__all_0s(&bstream_objs[tx_dump], LOOP_STA, RF_Hi);
	bstream__all_0s(&bstream_objs[tx_charge], LOOP_STA, RF_Hi);

	bstream__all_1s(&bstream_objs[tx_aux], LOOP_STO, RF_Lo);
	if (mode == 0x01) {
		bstream__all_0s(&bstream_objs[tx_h1], LOOP_STO, RF_Lo);
		bstream__all_1s(&bstream_objs[tx_h2], LOOP_STO, RF_Hi);
		bstream__all_1s(&bstream_objs[tx_l1], LOOP_STO, RF_Hi);
		bstream__all_0s(&bstream_objs[tx_l2], LOOP_STO, RF_Lo);
	}
	else if (mode == 0x02) {
		bstream__all_0s(&bstream_objs[tx_h1], LOOP_STO, RF_Lo - ( ( RF_Hi - RF_Lo ) >> 2 ));
		bstream__all_1s(&bstream_objs[tx_h2], LOOP_STO, RF_Hi + ( ( RF_Hi - RF_Lo ) >> 2 ));
		bstream__all_1s(&bstream_objs[tx_l1], LOOP_STO, RF_Hi);
		bstream__all_0s(&bstream_objs[tx_l2], LOOP_STO, RF_Lo);
	}
	else if (mode == 0x03) {
		bstream__all_0s(&bstream_objs[tx_h1], LOOP_STO, RF_Lo);
		bstream__all_1s(&bstream_objs[tx_h2], LOOP_STO, RF_Hi);
		bstream__all_1s(&bstream_objs[tx_l1], LOOP_STO, RF_Hi + ( ( RF_Hi - RF_Lo ) >> 2 ));
		bstream__all_0s(&bstream_objs[tx_l2], LOOP_STO, RF_Lo - ( ( RF_Hi - RF_Lo ) >> 2 ));
	}

	bstream__all_0s(&bstream_objs[tx_damp], LOOP_STO, RF_Lo);
	bstream__all_0s(&bstream_objs[tx_dump], LOOP_STO, RF_Lo);
	bstream__all_0s(&bstream_objs[tx_charge], LOOP_STO, RF_Lo);

	// dump all the current
	bstream__all_0s_us(&bstream_objs[tx_aux], NORMAL, dump_len_us);
	bstream__all_0s_us(&bstream_objs[tx_h1], NORMAL, dump_len_us);
	bstream__all_0s_us(&bstream_objs[tx_h2], NORMAL, dump_len_us);
	bstream__all_0s_us(&bstream_objs[tx_l1], NORMAL, dump_len_us);
	bstream__all_0s_us(&bstream_objs[tx_l2], NORMAL, dump_len_us);
	bstream__all_1s_us(&bstream_objs[tx_damp], NORMAL, dump_len_us);
	bstream__all_1s_us(&bstream_objs[tx_dump], NORMAL, dump_len_us);
	bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, dump_len_us);

	// turn-off everything
	double tail_us = 100;
	bstream__all_1s_us(&bstream_objs[tx_aux], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_h1], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_h2], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_l1], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_l2], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_damp], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_dump], NORMAL, tail_us);
	bstream__all_0s_us(&bstream_objs[tx_charge], NORMAL, tail_us);

	// end of sequence
	bstream__end_of_seq (&bstream_objs[tx_aux]);
	bstream__end_of_seq(&bstream_objs[tx_h1]);
	bstream__end_of_seq(&bstream_objs[tx_h2]);
	bstream__end_of_seq(&bstream_objs[tx_l1]);
	bstream__end_of_seq(&bstream_objs[tx_l2]);
	bstream__end_of_seq(&bstream_objs[tx_damp]);
	bstream__end_of_seq(&bstream_objs[tx_dump]);
	bstream__end_of_seq(&bstream_objs[tx_charge]);

	bstream_start();

}
