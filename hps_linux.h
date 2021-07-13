/*
 * hps_linux.h
 *
 *  Created on: May 26, 2021
 *      Author: David Ariando
 */

#ifndef HPS_LINUX_H_
#define HPS_LINUX_H_

// general system libraries
#include <alt_generalpurpose_io.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <hwlib.h>
#include <math.h>
#include <socal/alt_gpio.h>
#include <socal/hps.h>
#include <socal/socal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// local libraries
#include "functions/general.h"
#include "functions/reconfig_functions.h"
#include "functions/pll_param_generator.h"
#include "hps_soc_system.h"
#include "hps_functions.h"
#include "functions/bstream.h"
#include "functions/avalon_spi.h"
#include "functions/dac_ad5722r_driver.h"

#endif /* HPS_LINUX_H_ */
