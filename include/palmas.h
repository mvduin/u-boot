/*
 * (C) Copyright 2012-2013
 * Texas Instruments, <www.ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef PALMAS_H
#define PALMAS_H

#include <common.h>
#include <i2c.h>

/* I2C chip addresses, TW6035/37 */
#define TWL603X_CHIP_P1		0x48	/* Page 1 */
#define TWL603X_CHIP_P2		0x49	/* Page 2 */
#define TWL603X_CHIP_P3		0x4a	/* Page 3 */

/* TPS659038/39 */
#define TPS65903X_CHIP_P1	0x58	/* Page 1 */

/* Page 1 registers (0x1XY translates to page 1, reg addr 0xXY): */

/* LDO1 control/voltage */
#define LDO1_CTRL		0x50
#define LDO1_VOLTAGE		0x51

/* LDO2 control/voltage */
#define LDO2_CTRL		0x52
#define LDO2_VOLTAGE		0x53

/* LDO9 control/voltage */
#define LDO9_CTRL		0x60
#define LDO9_VOLTAGE		0x61

/* LDOUSB control/voltage */
#define LDOUSB_CTRL		0x64
#define LDOUSB_VOLTAGE		0x65
#define LDO_CTRL		0x6a

/* Control of 32 kHz audio clock */
#define CLK32KGAUDIO_CTRL	0xd5

/* SYSEN2_CTRL for VCC_3v3_AUX supply on the sEVM */
#define SYSEN2_CTRL		0xd9

/*
 * Bit field definitions for LDOx_CTRL, SYSENx_CTRL
 * and some other xxx_CTRL resources:
 */
#define RSC_STAT_ON		(1 << 4)	/* RO status bit! */
#define RSC_MODE_SLEEP		(1 << 2)
#define RSC_MODE_ACTIVE		(1 << 0)

/* LDO flags */
#define LDO_PERSIST		(1 << 7)	/* warm-reset insensitive */
#define LDO6_VIB_EN		(1 << 6)	/* LDO6: config for vibrator */
#define LDO8_TRACKING_EN	(1 << 6)	/* LDO8: tracking.. (what?) */
#define LDO9_BYP_EN		(1 << 6)	/* LDO9: bypass mode */

/* Some LDO voltage values */
#define LDO_VOLT_OFF		0
#define LDO_VOLT_0V9		0x01
#define LDO_VOLT_1V2		0x07
#define LDO_VOLT_1V5		0x0d
#define LDO_VOLT_1V8		0x13
#define LDO_VOLT_2V0		0x17
#define LDO_VOLT_2V8		0x27
#define LDO_VOLT_3V0		0x2b
#define LDO_VOLT_3V25		0x30
#define LDO_VOLT_3V3		0x31
/* Request bypass, LDO9 only */
#define LDO9_BYPASS		(LDO_VOLT_3V3 | LDO9_BYP_EN)

/* SMPS7_CTRL */
#define SMPS7_CTRL		0x30

/* SMPS9_CTRL */
#define SMPS9_CTRL		0x38
#define SMPS9_VOLTAGE		0x3b

/* SMPS10_CTRL */
#define SMPS10_CTRL		0x3c
#define SMPS10_MODE_ACTIVE_D	0x0d

/* Bit field definitions for SMPSx_CTRL */
#define SMPS_MODE_ACT_AUTO	1
#define SMPS_MODE_ACT_ECO	2
#define SMPS_MODE_ACT_FPWM	3
#define SMPS_MODE_SLP_AUTO	(1 << 2)
#define SMPS_MODE_SLP_ECO	(2 << 2)
#define SMPS_MODE_SLP_FPWM	(3 << 2)

/*
 * Some popular SMPS voltages, all with RANGE=1; note
 * that RANGE cannot be changed on the fly
 */
#define SMPS_VOLT_OFF		0
#define SMPS_VOLT_1V2		0x90
#define SMPS_VOLT_1V8		0xae
#define SMPS_VOLT_2V1		0xbd
#define SMPS_VOLT_3V0		0xea
#define SMPS_VOLT_3V3		0xf9

/* Backup Battery & VRTC Control */
#define BB_VRTC_CTRL		0xa8
/* Bit definitions for BB_VRTC_CTRL */
#define VRTC_EN_SLP		(1 << 6)
#define VRTC_EN_OFF		(1 << 5)
#define VRTC_PWEN		(1 << 4)
#define BB_LOW_ICHRG		(1 << 3)
#define BB_HIGH_ICHRG		(0 << 3)
#define BB_VSEL_3V0		(0 << 1)
#define BB_VSEL_2V5		(1 << 1)
#define BB_VSEL_3V15		(2 << 1)
#define BB_VSEL_VBAT		(3 << 1)
#define BB_CHRG_EN		(1 << 0)


#ifndef CONFIG_SYS_I2C_PALMAS_BUS_NUM
#define CONFIG_SYS_I2C_PALMAS_BUS_NUM	(0)
#endif

#ifndef CONFIG_SYS_I2C_PALMAS_ADDR
#if defined(CONFIG_DRA7XX) || defined(CONFIG_AM57XX)
#define CONFIG_SYS_I2C_PALMAS_ADDR	TPS65903X_CHIP_P1
#else
#define CONFIG_SYS_I2C_PALMAS_ADDR	TWL603X_CHIP_P1
#endif
#endif


/*
 * Functions to read and write from TPS659038/TWL6035/TWL6037
 * or other Palmas family of TI PMICs
 */
static inline int palmas_write(u8 reg, u8 *buf, int len)
{
	int saved_bus = i2c_get_bus_num();
	int ret = i2c_set_bus_num(CONFIG_SYS_I2C_PALMAS_BUS_NUM);
	if (!ret)
		ret = i2c_write(CONFIG_SYS_I2C_PALMAS_ADDR, reg, 1, buf, len);
	i2c_set_bus_num(saved_bus);
	return ret;
}

static inline int palmas_read(u8 reg, u8 *buf, int len)
{
	int saved_bus = i2c_get_bus_num();
	int ret = i2c_set_bus_num(CONFIG_SYS_I2C_PALMAS_BUS_NUM);
	if (!ret)
		ret = i2c_read(CONFIG_SYS_I2C_PALMAS_ADDR, reg, 1, buf, len);
	i2c_set_bus_num(saved_bus);
	return ret;
}

static inline int palmas_write_u8(u8 reg, u8 val)
{
	return palmas_write(reg, &val, 1);
}
static inline int palmas_read_u8(u8 reg, u8 *val)
{
	return palmas_read(reg, val, 1);
}

/* for backwards compatibility */
static inline int palmas_i2c_write_u8(u8 chip_no, u8 reg, u8 val)
{
	if (chip_no != CONFIG_SYS_I2C_PALMAS_ADDR)
		return -ENODEV;
	return palmas_write_u8(reg, val);
}
static inline int palmas_i2c_read_u8(u8 chip_no, u8 reg, u8 *val)
{
	if (chip_no != CONFIG_SYS_I2C_PALMAS_ADDR)
		return -ENODEV;
	return palmas_read_u8(reg, val);
}

int palmas_config_ldo(int ldo, u8 config);

void palmas_init_settings(void);
int palmas_mmc1_poweron_ldo(void);
int twl603x_audio_power(u8 on);
int twl603x_enable_bb_charge(u8 bb_fields);
int palmas_enable_ss_ldo(void);

#endif /* PALMAS_H */
