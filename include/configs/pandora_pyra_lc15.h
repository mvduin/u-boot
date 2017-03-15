/*
 * (C) Copyright 2013
 * Texas Instruments Incorporated.
 * Sricharan R	  <r.sricharan@ti.com>
 *
 * Configuration settings for the TI EVM5430 board.
 * See ti_omap5_common.h for omap5 common settings.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_PANDORA_PYRA_LC15_H
#define __CONFIG_PANDORA_PYRA_LC15_H

#include <configs/letux_cortex15.h>

/* Enable support for the TCA642X GPIO we have on the Pyra mainbord */
#define CONFIG_TCA642X
#define CONFIG_CMD_TCA642X
#define CONFIG_SYS_I2C_TCA642X_BUS_NUM 4
#define CONFIG_SYS_I2C_TCA642X_ADDR 0x22

#define CONFIG_SPL_ABORT_ON_RAW_IMAGE 1

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"run findfdt; " \
	"gpio -q clear 230; " \
	"i2c mw 16b 04 93; " \
	"i2c mw 048 a9 30; " \
	"i2c mw 049 16 8a; " \
	"i2c mw 464 ff 01 2; " \
	"i2c mw 464 07 10 2; " \
	"i2c mw 464 10 0; " \
	"tca642x input 05; " \
	"tca642x input 25; " \
	/* break on topright shoulder button */ \
	"gpio -q input 179 && exit; " \
	"run bootcmd_mmc0; " \
	"i2c mw 464 07 00 2; " \
	"i2c mw 464 0d 10 2; " \
	"i2c mw 464 10 0; " \
	"run bootcmd_mmc1; " \
	"tca642x output 05 0; " \
	"i2c mw 464 ff 01 2; " \
	""

#endif /* __CONFIG_PANDORA_PYRA_LC15_H */
