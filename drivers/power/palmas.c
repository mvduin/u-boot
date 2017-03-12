/*
 * (C) Copyright 2012-2013
 * Texas Instruments, <www.ti.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <config.h>
#include <palmas.h>

static const char *const ldo_names[12] = {
	"LDO1", "LDO2", "LDO3", "LDO4",
	"LDO5", "LDO6", "LDO7", "LDO8",
	"LDO9", "LDOLN", "LDOUSB", "LDOVANA",
};

int palmas_config_ldo(int ldo, u8 config)
{
	int err;
	u8 buf[2];
	buf[0] = config & 0xc0;  /* ldo flags */
	buf[1] = config & 0x3f;  /* voltage */
	if (buf[1] != 0)
		buf[0] |= RSC_MODE_SLEEP | RSC_MODE_ACTIVE;

	/* can't config LDOVANA */
	if (ldo < 1 || ldo >= 12) {
		printf("palmas_config_ldo: invalid ldo index %d\n", ldo);
		return 1;
	}
	--ldo;
	err = palmas_write(0x50 + 2 * ldo, buf, 2);
	if (err)
		printf("palmas: could not configure %s (ctrl=0x%x, "
				"vsel=%d)\n", ldo_names[ldo], buf[0], buf[1]);
	return err;
}

void palmas_init_settings(void)
{
#ifdef CONFIG_PALMAS_SMPS7_FPWM
	int err;
	/*
	 * Set SMPS7 (1.8 V I/O supply on platforms with TWL6035/37) to
	 * forced PWM mode. This reduces noise (but affects efficiency).
	 */
	u8 val = SMPS_MODE_SLP_FPWM | SMPS_MODE_ACT_FPWM;
	err = palmas_write_u8(SMPS7_CTRL, val);
	if (err)
		printf("palmas: could not force PWM for SMPS7: err = %d\n",
		       err);
#endif
}

int palmas_mmc1_poweron_ldo(void)
{
#if defined(CONFIG_DRA7XX) || defined(CONFIG_AM57XX)
	/*
	 * Currently valid for the dra7xx_evm board:
	 * Set TPS659038 LDO1 to 3.0 V
	 */
	return palmas_config_ldo(1, LDO_VOLT_3V0);
#else
	/*
	 * We assume that this is a OMAP543X + TWL603X board:
	 * Set TWL6035/37 LDO9 to 3.0 V
	 */
	return palmas_config_ldo(9, LDO_VOLT_3V0);
#endif
}

#ifdef CONFIG_PALMAS_AUDPWR
/*
 * Turn audio codec power and 32 kHz clock on/off. Use for
 * testing OMAP543X + TWL603X + TWL604X boards only.
 */
int twl603x_audio_power(u8 on)
{
	u8 cval = 0, vval = 0, c32k = 0;
	int err;

	if (on) {
		vval = SMPS_VOLT_2V1;
		cval = SMPS_MODE_SLP_AUTO | SMPS_MODE_ACT_AUTO;
		c32k = RSC_MODE_SLEEP | RSC_MODE_ACTIVE;
	}
	/* Set SMPS9 to 2.1 V (for TWL604x), or to 0 (off) */
	err = palmas_write_u8(SMPS9_VOLTAGE, vval);
	if (err) {
		printf("twl603x: could not set SMPS9 voltage: err = %d\n",
		       err);
		return err;
	}
	/* Turn on or off SMPS9 */
	err = palmas_write_u8(SMPS9_CTRL, cval);
	if (err) {
		printf("twl603x: could not turn SMPS9 %s: err = %d\n",
		       cval ? "on" : "off", err);
		return err;
	}
	/* Output 32 kHz clock on or off */
	err = palmas_write_u8(CLK32KGAUDIO_CTRL, c32k);
	if (err)
		printf("twl603x: could not turn CLK32KGAUDIO %s: err = %d\n",
		       c32k ? "on" : "off", err);
	return err;
}
#endif

#ifdef CONFIG_PALMAS_USB_SS_PWR
/**
 * @brief palmas_enable_ss_ldo - Configure EVM board specific configurations
 * for the USB Super speed SMPS10 regulator.
 *
 * @return 0
 */
int palmas_enable_ss_ldo(void)
{
	/* Enable smps10 regulator  */
	return palmas_write_u8(SMPS10_CTRL,
				SMPS10_MODE_ACTIVE_D);
}
#endif

/*
 * Enable/disable back-up battery (or super cap) charging on TWL6035/37.
 * Please use defined BB_xxx values.
 */
int twl603x_enable_bb_charge(u8 bb_fields)
{
	u8 val = bb_fields & 0x0f;
	int err;

	val |= (VRTC_EN_SLP | VRTC_EN_OFF | VRTC_PWEN);
	err = palmas_write_u8(BB_VRTC_CTRL, val);
	if (err)
		printf("twl603x: could not set BB_VRTC_CTRL to 0x%02x: err = %d\n",
		       val, err);
	return err;
}

/*
 * power off TWL6035/37.
 */
int twl603x_poweroff(bool restart)
{
	printf( "### %s ###\n", restart ? "HARDRESET" : "POWEROFF" );
	return palmas_i2c_write_u8(TWL603X_CHIP_P1, 0xa0, restart?0x03:0x00);
}
