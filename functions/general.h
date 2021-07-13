// offsets for output control signal
#define RXDAC_CLRn_ofst					(8)
#define RXDAC_LDACn_ofst				(7)
#define BITSTR_ADV_RST_ofst				(6)
#define	BITSTR_ADV_START_ofst			(5)
#define SYS_PLL_RST_ofst				(4)
#define BITSTR_STOP_ofst				(3)
#define BITSTR_RST_ofst					(2)
#define BITSTR_START_ofst				(1)
#define BITSTR_FIFO_RST_ofst			(0)

// Output control signal to FPGA
#define RXDAC_CLRn					(1<<RXDAC_CLRn_ofst)
#define RXDAC_LDACn					(1<<RXDAC_LDACn_ofst)
#define BITSTR_ADV_RST				(1<<BITSTR_ADV_RST_ofst)
#define	BITSTR_ADV_START			(1<<BITSTR_ADV_START_ofst)
#define SYS_PLL_RST					(1<<SYS_PLL_RST_ofst_ofst)
#define BITSTR_STOP					(1<<BITSTR_STOP_ofst)
#define BITSTR_RST					(1<<BITSTR_RST_ofst)
#define BITSTR_START				(1<<BITSTR_START_ofst)
#define BITSTR_FIFO_RST				(1<<BITSTR_FIFO_RST_ofst)

// default value for cnt_out
#define CNT_OUT_default		(0)

// Offsets for input status signal
#define BITSTR_END_ofst			(1)
#define PLL_SYS_lock_ofst		(0)

// Input status signal from FPGA
#define BITSTR_END				(1<<BITSTR_END_ofst)
#define PLL_SYS_lock			(1<<PLL_SYS_lock_ofst)

// general variable
#define ENABLE_MESSAGE	1
#define DISABLE_MESSAGE 0

// error
#define SEQ_ERROR	1
#define SEQ_OK		0

#define ENABLE 1
#define DISABLE 0

#define STORE_TO_SDRAM_NOREAD 1			// store to sdram, but don't store it in a file
#define RD_DATA_VIA_SDRAM_OR_FIFO 0		// store to a file, either via SDRAM read or FIFO read

#define RD_SDRAM 1						// read via SDRAM, must be used with RD_DATA_VIA_SDRAM_OR_FIFO
#define RD_FIFO 0						// read via FIFO, must be used with RD_DATA_VIA_SDRAM_OR_FIFO
