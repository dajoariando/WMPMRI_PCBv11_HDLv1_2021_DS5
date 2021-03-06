#ifndef _ALTERA_HPS_SOC_SYSTEM_H_
#define _ALTERA_HPS_SOC_SYSTEM_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'soc_system' in
 * file '../Quartus_SoC/soc_system.sopcinfo'.
 */

/*
 * This file contains macros for module 'hps_0' and devices
 * connected to the following masters:
 *   h2f_axi_master
 *   h2f_lw_axi_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'tx_DUMP', class 'bstream'
 * The macros are prefixed with 'TX_DUMP_'.
 * The prefix is the slave descriptor.
 */
#define TX_DUMP_COMPONENT_TYPE bstream
#define TX_DUMP_COMPONENT_NAME tx_DUMP
#define TX_DUMP_BASE 0x0
#define TX_DUMP_SPAN 4096
#define TX_DUMP_END 0xfff

/*
 * Macros for device 'sys_pll_reconfig', class 'altera_pll_reconfig'
 * The macros are prefixed with 'SYS_PLL_RECONFIG_'.
 * The prefix is the slave descriptor.
 */
#define SYS_PLL_RECONFIG_COMPONENT_TYPE altera_pll_reconfig
#define SYS_PLL_RECONFIG_COMPONENT_NAME sys_pll_reconfig
#define SYS_PLL_RECONFIG_BASE 0x0
#define SYS_PLL_RECONFIG_SPAN 256
#define SYS_PLL_RECONFIG_END 0xff

/*
 * Macros for device 'rx_dac', class 'altera_avalon_spi'
 * The macros are prefixed with 'RX_DAC_'.
 * The prefix is the slave descriptor.
 */
#define RX_DAC_COMPONENT_TYPE altera_avalon_spi
#define RX_DAC_COMPONENT_NAME rx_dac
#define RX_DAC_BASE 0x100
#define RX_DAC_SPAN 32
#define RX_DAC_END 0x11f
#define RX_DAC_IRQ 3
#define RX_DAC_CLOCKMULT 1
#define RX_DAC_CLOCKPHASE 0
#define RX_DAC_CLOCKPOLARITY 1
#define RX_DAC_CLOCKUNITS "Hz"
#define RX_DAC_DATABITS 24
#define RX_DAC_DATAWIDTH 32
#define RX_DAC_DELAYMULT "1.0E-9"
#define RX_DAC_DELAYUNITS "ns"
#define RX_DAC_EXTRADELAY 0
#define RX_DAC_INSERT_SYNC 0
#define RX_DAC_ISMASTER 1
#define RX_DAC_LSBFIRST 0
#define RX_DAC_NUMSLAVES 1
#define RX_DAC_PREFIX "spi_"
#define RX_DAC_SYNC_REG_DEPTH 2
#define RX_DAC_TARGETCLOCK 128000
#define RX_DAC_TARGETSSDELAY "0.0"

/*
 * Macros for device 'cnt_in', class 'altera_avalon_pio'
 * The macros are prefixed with 'CNT_IN_'.
 * The prefix is the slave descriptor.
 */
#define CNT_IN_COMPONENT_TYPE altera_avalon_pio
#define CNT_IN_COMPONENT_NAME cnt_in
#define CNT_IN_BASE 0x120
#define CNT_IN_SPAN 16
#define CNT_IN_END 0x12f
#define CNT_IN_BIT_CLEARING_EDGE_REGISTER 0
#define CNT_IN_BIT_MODIFYING_OUTPUT_REGISTER 0
#define CNT_IN_CAPTURE 0
#define CNT_IN_DATA_WIDTH 32
#define CNT_IN_DO_TEST_BENCH_WIRING 0
#define CNT_IN_DRIVEN_SIM_VALUE 0
#define CNT_IN_EDGE_TYPE NONE
#define CNT_IN_FREQ 50000000
#define CNT_IN_HAS_IN 1
#define CNT_IN_HAS_OUT 0
#define CNT_IN_HAS_TRI 0
#define CNT_IN_IRQ_TYPE NONE
#define CNT_IN_RESET_VALUE 0

/*
 * Macros for device 'cnt_out', class 'altera_avalon_pio'
 * The macros are prefixed with 'CNT_OUT_'.
 * The prefix is the slave descriptor.
 */
#define CNT_OUT_COMPONENT_TYPE altera_avalon_pio
#define CNT_OUT_COMPONENT_NAME cnt_out
#define CNT_OUT_BASE 0x130
#define CNT_OUT_SPAN 16
#define CNT_OUT_END 0x13f
#define CNT_OUT_BIT_CLEARING_EDGE_REGISTER 0
#define CNT_OUT_BIT_MODIFYING_OUTPUT_REGISTER 0
#define CNT_OUT_CAPTURE 0
#define CNT_OUT_DATA_WIDTH 32
#define CNT_OUT_DO_TEST_BENCH_WIRING 0
#define CNT_OUT_DRIVEN_SIM_VALUE 0
#define CNT_OUT_EDGE_TYPE NONE
#define CNT_OUT_FREQ 50000000
#define CNT_OUT_HAS_IN 0
#define CNT_OUT_HAS_OUT 1
#define CNT_OUT_HAS_TRI 0
#define CNT_OUT_IRQ_TYPE NONE
#define CNT_OUT_RESET_VALUE 0

/*
 * Macros for device 'tx_DAMP', class 'bstream'
 * The macros are prefixed with 'TX_DAMP_'.
 * The prefix is the slave descriptor.
 */
#define TX_DAMP_COMPONENT_TYPE bstream
#define TX_DAMP_COMPONENT_NAME tx_DAMP
#define TX_DAMP_BASE 0x1000
#define TX_DAMP_SPAN 4096
#define TX_DAMP_END 0x1fff

/*
 * Macros for device 'tx_CHRG', class 'bstream'
 * The macros are prefixed with 'TX_CHRG_'.
 * The prefix is the slave descriptor.
 */
#define TX_CHRG_COMPONENT_TYPE bstream
#define TX_CHRG_COMPONENT_NAME tx_CHRG
#define TX_CHRG_BASE 0x2000
#define TX_CHRG_SPAN 4096
#define TX_CHRG_END 0x2fff

/*
 * Macros for device 'tx_L2', class 'bstream'
 * The macros are prefixed with 'TX_L2_'.
 * The prefix is the slave descriptor.
 */
#define TX_L2_COMPONENT_TYPE bstream
#define TX_L2_COMPONENT_NAME tx_L2
#define TX_L2_BASE 0x3000
#define TX_L2_SPAN 4096
#define TX_L2_END 0x3fff

/*
 * Macros for device 'tx_L1', class 'bstream'
 * The macros are prefixed with 'TX_L1_'.
 * The prefix is the slave descriptor.
 */
#define TX_L1_COMPONENT_TYPE bstream
#define TX_L1_COMPONENT_NAME tx_L1
#define TX_L1_BASE 0x4000
#define TX_L1_SPAN 4096
#define TX_L1_END 0x4fff

/*
 * Macros for device 'tx_H2', class 'bstream'
 * The macros are prefixed with 'TX_H2_'.
 * The prefix is the slave descriptor.
 */
#define TX_H2_COMPONENT_TYPE bstream
#define TX_H2_COMPONENT_NAME tx_H2
#define TX_H2_BASE 0x5000
#define TX_H2_SPAN 4096
#define TX_H2_END 0x5fff

/*
 * Macros for device 'tx_H1', class 'bstream'
 * The macros are prefixed with 'TX_H1_'.
 * The prefix is the slave descriptor.
 */
#define TX_H1_COMPONENT_TYPE bstream
#define TX_H1_COMPONENT_NAME tx_H1
#define TX_H1_BASE 0x6000
#define TX_H1_SPAN 4096
#define TX_H1_END 0x6fff

/*
 * Macros for device 'tx_aux', class 'bstream'
 * The macros are prefixed with 'TX_AUX_'.
 * The prefix is the slave descriptor.
 */
#define TX_AUX_COMPONENT_TYPE bstream
#define TX_AUX_COMPONENT_NAME tx_aux
#define TX_AUX_BASE 0x7000
#define TX_AUX_SPAN 4096
#define TX_AUX_END 0x7fff

/*
 * Macros for device 'rx_in_short', class 'bstream'
 * The macros are prefixed with 'RX_IN_SHORT_'.
 * The prefix is the slave descriptor.
 */
#define RX_IN_SHORT_COMPONENT_TYPE bstream
#define RX_IN_SHORT_COMPONENT_NAME rx_in_short
#define RX_IN_SHORT_BASE 0x8000
#define RX_IN_SHORT_SPAN 4096
#define RX_IN_SHORT_END 0x8fff

/*
 * Macros for device 'rx_inc_damp', class 'bstream'
 * The macros are prefixed with 'RX_INC_DAMP_'.
 * The prefix is the slave descriptor.
 */
#define RX_INC_DAMP_COMPONENT_TYPE bstream
#define RX_INC_DAMP_COMPONENT_NAME rx_inc_damp
#define RX_INC_DAMP_BASE 0x9000
#define RX_INC_DAMP_SPAN 4096
#define RX_INC_DAMP_END 0x9fff

/*
 * Macros for device 'led_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'LED_PIO_'.
 * The prefix is the slave descriptor.
 */
#define LED_PIO_COMPONENT_TYPE altera_avalon_pio
#define LED_PIO_COMPONENT_NAME led_pio
#define LED_PIO_BASE 0x10040
#define LED_PIO_SPAN 16
#define LED_PIO_END 0x1004f
#define LED_PIO_BIT_CLEARING_EDGE_REGISTER 0
#define LED_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_PIO_CAPTURE 0
#define LED_PIO_DATA_WIDTH 10
#define LED_PIO_DO_TEST_BENCH_WIRING 0
#define LED_PIO_DRIVEN_SIM_VALUE 0
#define LED_PIO_EDGE_TYPE NONE
#define LED_PIO_FREQ 50000000
#define LED_PIO_HAS_IN 0
#define LED_PIO_HAS_OUT 1
#define LED_PIO_HAS_TRI 0
#define LED_PIO_IRQ_TYPE NONE
#define LED_PIO_RESET_VALUE 15

/*
 * Macros for device 'dipsw_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'DIPSW_PIO_'.
 * The prefix is the slave descriptor.
 */
#define DIPSW_PIO_COMPONENT_TYPE altera_avalon_pio
#define DIPSW_PIO_COMPONENT_NAME dipsw_pio
#define DIPSW_PIO_BASE 0x10080
#define DIPSW_PIO_SPAN 16
#define DIPSW_PIO_END 0x1008f
#define DIPSW_PIO_IRQ 0
#define DIPSW_PIO_BIT_CLEARING_EDGE_REGISTER 1
#define DIPSW_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DIPSW_PIO_CAPTURE 1
#define DIPSW_PIO_DATA_WIDTH 10
#define DIPSW_PIO_DO_TEST_BENCH_WIRING 0
#define DIPSW_PIO_DRIVEN_SIM_VALUE 0
#define DIPSW_PIO_EDGE_TYPE ANY
#define DIPSW_PIO_FREQ 50000000
#define DIPSW_PIO_HAS_IN 1
#define DIPSW_PIO_HAS_OUT 0
#define DIPSW_PIO_HAS_TRI 0
#define DIPSW_PIO_IRQ_TYPE EDGE
#define DIPSW_PIO_RESET_VALUE 0

/*
 * Macros for device 'button_pio', class 'altera_avalon_pio'
 * The macros are prefixed with 'BUTTON_PIO_'.
 * The prefix is the slave descriptor.
 */
#define BUTTON_PIO_COMPONENT_TYPE altera_avalon_pio
#define BUTTON_PIO_COMPONENT_NAME button_pio
#define BUTTON_PIO_BASE 0x100c0
#define BUTTON_PIO_SPAN 16
#define BUTTON_PIO_END 0x100cf
#define BUTTON_PIO_IRQ 1
#define BUTTON_PIO_BIT_CLEARING_EDGE_REGISTER 1
#define BUTTON_PIO_BIT_MODIFYING_OUTPUT_REGISTER 0
#define BUTTON_PIO_CAPTURE 1
#define BUTTON_PIO_DATA_WIDTH 4
#define BUTTON_PIO_DO_TEST_BENCH_WIRING 0
#define BUTTON_PIO_DRIVEN_SIM_VALUE 0
#define BUTTON_PIO_EDGE_TYPE FALLING
#define BUTTON_PIO_FREQ 50000000
#define BUTTON_PIO_HAS_IN 1
#define BUTTON_PIO_HAS_OUT 0
#define BUTTON_PIO_HAS_TRI 0
#define BUTTON_PIO_IRQ_TYPE EDGE
#define BUTTON_PIO_RESET_VALUE 0


#endif /* _ALTERA_HPS_SOC_SYSTEM_H_ */
