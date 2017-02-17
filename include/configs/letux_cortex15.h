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

#ifndef __CONFIG_LETUX_CORTEX15_H
#define __CONFIG_LETUX_CORTEX15_H

#define CONFIG_NR_DRAM_BANKS 2

/* MMC ENV related defines */
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		1	/* SLOT2: eMMC(1) */
#define CONFIG_ENV_SIZE			(128 << 10)
#define CONFIG_ENV_OFFSET		0xE0000
#define CONFIG_ENV_OFFSET_REDUND	(CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT

#define CONFIG_CONS_INDEX		3
#define CONSOLEDEV			"ttyO2"
#define CONFIG_SYS_NS16550_COM3		UART3_BASE
#define CONFIG_BAUDRATE			115200

#ifndef CONFIG_SPL_BUILD
/* Define the default GPT table for eMMC */
#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=rootfs,start=2MiB,size=-,uuid=${uuid_gpt_rootfs}"
#endif

#if !defined(CONFIG_SPL_BUILD) || defined(CONFIG_SPL_DFU_SUPPORT)
#define DFU_ALT_INFO_RAM \
	"dfu_alt_info_ram=" \
	"kernel ram 0x80200000 0x4000000;" \
	"fdt ram 0x80f80000 0x80000;" \
	"ramdisk ram 0x81000000 0x4000000\0"
#define DFUARGS \
	"dfu_bufsiz=0x10000\0" \
	DFU_ALT_INFO_RAM
#endif

#ifdef CONFIG_SPL_BUILD
#undef CONFIG_CMD_BOOTD
#ifdef CONFIG_SPL_DFU_SUPPORT
#define CONFIG_SPL_LOAD_FIT_ADDRESS 0x80200000
#define CONFIG_SPL_HASH_SUPPORT
#endif
#endif

#include <configs/ti_omap5_common.h>

#define CONFIG_MISC_INIT_R

/* Enhance our eMMC support / experience. */
#define CONFIG_HSMMC2_8BIT
#define CONFIG_SUPPORT_EMMC_BOOT

/* USB UHH support options */
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_OMAP
#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS 3
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET

#define CONFIG_OMAP_EHCI_PHY2_RESET_GPIO 80

#ifdef CONFIG_USB_GADGET
#define CONFIG_USB_FUNCTION_MASS_STORAGE
#endif

/* Max time to hold reset on this board, see doc/README.omap-reset-time */
#define CONFIG_OMAP_PLATFORM_RESET_TIME_MAX_USEC	16296

/* SATA */
#define CONFIG_SCSI
#define CONFIG_LIBATA
#define CONFIG_SCSI_AHCI
#define CONFIG_SCSI_AHCI_PLAT
#define CONFIG_SYS_SCSI_MAX_SCSI_ID	1
#define CONFIG_SYS_SCSI_MAX_LUN		1
#define CONFIG_SYS_SCSI_MAX_DEVICE	(CONFIG_SYS_SCSI_MAX_SCSI_ID * \
						CONFIG_SYS_SCSI_MAX_LUN)

#ifdef CONFIG_SPL_BUILD
#undef CONFIG_DM_SPI
#undef CONFIG_DM_SPI_FLASH
#endif

#define CONFIG_CMD_POWEROFF	1
#endif /* __CONFIG_LETUX_CORTEX15_H */
