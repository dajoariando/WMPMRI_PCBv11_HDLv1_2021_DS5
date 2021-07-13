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

#define PATTERN_WIDTH 120 // the pattern width is set in the Quartus, check if it match

typedef struct bstream_struct {
	volatile unsigned int * sram_addr;   // the sram address
	unsigned int curr_ofst;		// the sram current offset
	char loop_sta;				// the loop start flag
	char loop_sto;				// the loop stop flag
	char end_of_seq;			// the end of sequence flag
	unsigned long repetition;	// the number of repetition
	float freq_MHz;					// frequency
	char error_seq;				// the error flag
} bstream_obj;

// define the variables for the bitstream
enum bstream_gpio {
	tx_h1 = 0, tx_l1, tx_h2, tx_l2, tx_charge, tx_damp, tx_dump, tx_aux, rx_inc_damp, rx_in_short, BSTREAM_COUNT   // BSTREAM_COUNT is a dummy variable to mark the end of the enum
};
extern bstream_obj bstream_objs[BSTREAM_COUNT];

void bstream__push(bstream_obj * obj, char pattern_mode, char all_1s_mode, char all_0s_mode, char loop_sta, char loop_sto, char end_of_seq, unsigned long long dat_msb, unsigned long long dat_lsb);

void bstream__init_all_sram();
void bstream__init(bstream_obj *obj, unsigned long repetition, float freq_MHz);

void bstream_rst();

void bstream_start();

char bstream_check(bstream_obj *obj);

void bstream__end_of_seq(bstream_obj *obj);

void bstream__all_1s(bstream_obj *obj, char mode, unsigned long len);
void bstream__all_1s_us(bstream_obj *obj, char mode, float len_us);

void bstream__all_0s(bstream_obj *obj, char mode, unsigned long len);
void bstream__all_0s_us(bstream_obj *obj, char mode, float len_us);

void bstream__pattern(bstream_obj *obj, char mode, unsigned long long dat_msb, unsigned long long dat_lsb);

void bstream__null_output(bstream_obj *obj);

void bstream__toggle(bstream_obj *obj, float freq_MHz, float pulse_us, unsigned long repetition);

// initialized-object functions
void bstream__null_everything();   // null all outputs of the registered bstream
void bstream__prechrg_n_dump(float clk_MHz, double bstrap_pchg_us, double ind_pchg_us, double tail_us, double dump_dly_us, double dump_len_us, unsigned char en_pchrg);   // precharge the circuit and dump
void bstream__prechrg_n_rf_n_dump(float clk_MHz, float RFclk_MHz, double dtcl, double ind_pchg_us, double dump_len_us, unsigned char en_pchrg, unsigned int repetition);

#endif /* FUNCTIONS_BSTREAM_H_ */
