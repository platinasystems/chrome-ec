/* Copyright 2019 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Volteer baseboard configuration */

#ifndef __CROS_EC_BASEBOARD_H
#define __CROS_EC_BASEBOARD_H

/*
 * By default, enable all console messages excepted HC
 */
#define CC_DEFAULT     (CC_ALL & ~(BIT(CC_HOSTCMD)))

/* NPCX7 config */
#define NPCX7_PWM1_SEL    1  /* GPIO C2 is used as PWM1. */
#define NPCX_UART_MODULE2 1  /* GPIO64/65 are used as UART pins. */
/* Internal SPI flash on NPCX796FC is 512 kB */
#define CONFIG_FLASH_SIZE (512 * 1024)
#define CONFIG_SPI_FLASH_REGS
#define CONFIG_SPI_FLASH_W25Q80 /* Internal SPI flash type. */

/* EC Defines */
#define CONFIG_LTO
#define CONFIG_BOARD_VERSION_CBI
#define CONFIG_CRC8
#define CONFIG_CROS_BOARD_INFO
#define CONFIG_HIBERNATE_PSL
#define CONFIG_PWM
#define CONFIG_VBOOT_HASH
#define CONFIG_VSTORE
#define CONFIG_VSTORE_SLOT_COUNT 1
#define CONFIG_VOLUME_BUTTONS
#define CONFIG_LOW_POWER_IDLE

/* Host communication */
#define CONFIG_HOSTCMD_ESPI
#define CONFIG_HOSTCMD_ESPI_VW_SLP_S4

/* Chipset config */
#define CONFIG_CHIPSET_TIGERLAKE
#define CONFIG_CHIPSET_PP3300_RAIL_FIRST
#define CONFIG_CHIPSET_X86_RSMRST_DELAY
#define CONFIG_CHIPSET_RESET_HOOK
#define CONFIG_CPU_PROCHOT_ACTIVE_LOW
#define CONFIG_EXTPOWER_GPIO
#define CONFIG_POWER_BUTTON
#define CONFIG_POWER_BUTTON_X86
#define CONFIG_POWER_COMMON
#define CONFIG_POWER_S0IX
#define CONFIG_POWER_S0IX_FAILURE_DETECTION
#define CONFIG_POWER_TRACK_HOST_SLEEP_STATE
#define CONFIG_BOARD_HAS_RTC_RESET

/* Common Keyboard Defines */
#define CONFIG_CMD_KEYBOARD
#define CONFIG_KEYBOARD_BOARD_CONFIG
#define CONFIG_KEYBOARD_COL2_INVERTED
#define CONFIG_KEYBOARD_KEYPAD
#define CONFIG_KEYBOARD_PROTOCOL_8042
#define CONFIG_KEYBOARD_PWRBTN_ASSERTS_KSI2
#define CONFIG_PWM_KBLIGHT


/* Sensors */
#define CONFIG_TABLET_MODE
#define CONFIG_GMR_TABLET_MODE

#define CONFIG_MKBP_EVENT
#define CONFIG_MKBP_USE_GPIO
#define CONFIG_DYNAMIC_MOTION_SENSOR_COUNT
#define CONFIG_ACCEL_INTERRUPTS

/* Enable sensor fifo, must also define the _SIZE and _THRES */
#define CONFIG_ACCEL_FIFO
/* FIFO size is in power of 2. */
#define CONFIG_ACCEL_FIFO_SIZE 256
/* Depends on how fast the AP boots and typical ODRs */
#define CONFIG_ACCEL_FIFO_THRES (CONFIG_ACCEL_FIFO_SIZE / 3)

/* Sensor console commands */
#define CONFIG_CMD_ACCELS
#define CONFIG_CMD_ACCEL_INFO

/* BMA253 accelerometer in base */
#define CONFIG_ACCEL_BMA255

/* Camera VSYNC */
#define CONFIG_SYNC
#define CONFIG_SYNC_INT_EVENT \
	TASK_EVENT_MOTION_SENSOR_INTERRUPT(VSYNC)

/* TCS3400 ALS */
#define CONFIG_ALS
#define ALS_COUNT		1
#define CONFIG_ALS_TCS3400
#define CONFIG_ALS_TCS3400_INT_EVENT \
	TASK_EVENT_MOTION_SENSOR_INTERRUPT(CLEAR_ALS)

/* Sensors without hardware FIFO are in forced mode */
#define CONFIG_ACCEL_FORCE_MODE_MASK \
	(BIT(LID_ACCEL) | BIT(CLEAR_ALS))

/* Thermal features */
#define CONFIG_FANS			FAN_CH_COUNT
#define CONFIG_TEMP_SENSOR
#define CONFIG_TEMP_SENSOR_POWER_GPIO	GPIO_EN_PP3300_A
#define CONFIG_THERMISTOR
#define CONFIG_STEINHART_HART_3V3_30K9_47K_4050B
#define CONFIG_THROTTLE_AP

/* Common charger defines */
#define CONFIG_CHARGE_MANAGER
#define CONFIG_CHARGE_RAMP_HW
#define CONFIG_CHARGER
#define CONFIG_CHARGER_DISCHARGE_ON_AC
#define CONFIG_CHARGER_INPUT_CURRENT		512
#define CONFIG_CHARGER_ISL9241
#define CONFIG_CHARGER_SENSE_RESISTOR		10
#define CONFIG_CHARGER_SENSE_RESISTOR_AC	10

#define CONFIG_USB_CHARGER
#define CONFIG_BC12_DETECT_PI3USB9201

/*
 * Don't allow the system to boot to S0 when the battery is low and unable to
 * communicate on locked systems (which haven't PD negotiated)
 */
#define CONFIG_CHARGER_MIN_POWER_MW_FOR_POWER_ON_WITH_BATT	15000
#define CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON			3
#define CONFIG_CHARGER_MIN_BAT_PCT_FOR_POWER_ON_WITH_AC		1
#define CONFIG_CHARGER_MIN_POWER_MW_FOR_POWER_ON		15001

/* Common battery defines */
#define CONFIG_BATTERY_SMART
#define CONFIG_BATTERY_FUEL_GAUGE
/* TODO: b/143809318 enable cut off */
/* #define CONFIG_BATTERY_CUT_OFF */

/* Common LED defines */
#define CONFIG_LED_COMMON
#define CONFIG_LED_PWM
/* Although there are 2 LEDs, they are both controlled by the same lines. */
#define CONFIG_LED_PWM_COUNT 1

/* USB Type C and USB PD defines */
/* Enable the new USB-C PD stack */
/* TODO: b/145756626 - re-enable once all blocking issues resolved */
#if 0
#define CONFIG_USB_PD_TCPMV2
#define CONFIG_USB_TYPEC_SM
#define CONFIG_USB_PRL_SM
#define CONFIG_USB_PE_SM
#define CONFIG_USB_TYPEC_DRP_ACC_TRYSRC
#else
/*
 * PD 3.0 is always enabled by the TCPMv2 stack, so it's only explicitly
 * enabled when using the TCPMv1 stack
 */
#define CONFIG_USB_PD_REV30
#endif

#define CONFIG_USB_POWER_DELIVERY
#define CONFIG_USB_PD_TCPMV1
#define CONFIG_USB_PD_ALT_MODE
#define CONFIG_USB_PD_ALT_MODE_DFP
#define CONFIG_USB_PD_DISCHARGE_PPC
#define CONFIG_USB_PD_DUAL_ROLE
#define CONFIG_USB_PD_MAX_SINGLE_SOURCE_CURRENT		TYPEC_RP_3A0
#define CONFIG_USB_PD_PORT_MAX_COUNT			2
#define CONFIG_USB_PD_TCPC_RUNTIME_CONFIG
/* TODO: b/145250123: Enabling low-power mode breaks USB SNK detection */
#undef CONFIG_USB_PD_TCPC_LOW_POWER
#define CONFIG_USB_PD_TCPM_TCPCI
#define CONFIG_USB_PD_TCPM_TUSB422	/* USBC port C0 */
#define CONFIG_USB_PD_TCPM_PS8815	/* USBC port USB3 DB */
#define CONFIG_USB_PD_TCPM_MUX
#define CONFIG_CMD_PD_CONTROL		/* Needed for TCPC FW update */
#define CONFIG_CMD_USB_PD_PE

#define CONFIG_USB_PD_TRY_SRC
#define CONFIG_USB_PD_VBUS_DETECT_PPC
#define CONFIG_USB_PD_VBUS_MEASURE_NOT_PRESENT

#define CONFIG_USBC_PPC
#define CONFIG_CMD_PPC_DUMP
/* Note - SN5S330 support automatically adds
 * CONFIG_USBC_PPC_POLARITY
 * CONFIG_USBC_PPC_SBU
 * CONFIG_USBC_PPC_VCONN
 */
#define CONFIG_USBC_PPC_SN5S330		/* USBC port C0 */
#define CONFIG_USBC_PPC_SYV682X		/* USBC port C1 */

#define CONFIG_INTEL_VIRTUAL_MUX
#define CONFIG_USBC_SS_MUX
#define CONFIG_USB_MUX_VIRTUAL

#define CONFIG_USBC_VCONN
#define CONFIG_USBC_VCONN_SWAP

/* Enabling SOP* communication */
#define CONFIG_CMD_USB_PD_CABLE
#define CONFIG_USB_PD_DECODE_SOP

/* Enabling Thunderbolt-compatible mode */
#define CONFIG_USB_PD_TBT_COMPAT_MODE

/* Enabling USB4 mode */
#define CONFIG_USB_PD_USB4

/*
 * USB ID
 * This is allocated specifically for Volteer
 * http://google3/hardware/standards/usb/
 */
#define CONFIG_USB_PID 0x503E

/* TODO: b/144165680 - measure and check these values on Volteer */
#define PD_POWER_SUPPLY_TURN_ON_DELAY	30000 /* us */
#define PD_POWER_SUPPLY_TURN_OFF_DELAY	30000 /* us */
#define PD_VCONN_SWAP_DELAY		5000 /* us */

/* Retimer */
#define CONFIG_USBC_MUX_RETIMER
#define CONFIG_USBC_RETIMER_INTEL_BB
#define CONFIG_USBC_RETIMER_INTEL_BB_RUNTIME_CONFIG
#define USBC_PORT_C1_BB_RETIMER_I2C_ADDR	0x40

/*
 * SN5S30 PPC supports up to 24V VBUS source and sink, however passive USB-C
 * cables only support up to 60W.
 */
#define PD_OPERATING_POWER_MW	15000
#define PD_MAX_POWER_MW		60000
#define PD_MAX_CURRENT_MA	3000
#define PD_MAX_VOLTAGE_MV	20000


/* I2C Bus Configuration */
#define CONFIG_I2C
#define I2C_PORT_SENSOR		NPCX_I2C_PORT0_0
#define I2C_PORT_USB_C0		NPCX_I2C_PORT1_0
#define I2C_PORT_USB_C1		NPCX_I2C_PORT2_0
#define I2C_PORT_USB_1_MIX	NPCX_I2C_PORT3_0
#define I2C_PORT_POWER		NPCX_I2C_PORT5_0
#define I2C_PORT_EEPROM		NPCX_I2C_PORT7_0

#define I2C_PORT_BATTERY	I2C_PORT_POWER
#define I2C_PORT_CHARGER	I2C_PORT_EEPROM

#define I2C_ADDR_EEPROM_FLAGS	0x50
#define CONFIG_I2C_MASTER


#ifndef __ASSEMBLER__

#include "gpio_signal.h"


enum adc_channel {
	ADC_TEMP_SENSOR_1_CHARGER,
	ADC_TEMP_SENSOR_2_PP3300_REGULATOR,
	ADC_TEMP_SENSOR_3_DDR_SOC,
	ADC_TEMP_SENSOR_4_FAN,
	ADC_CH_COUNT
};

enum pwm_channel {
	PWM_CH_LED1_BLUE = 0,
	PWM_CH_LED2_GREEN,
	PWM_CH_LED3_RED,
	PWM_CH_LED4_SIDESEL,
	PWM_CH_FAN,
	PWM_CH_KBLIGHT,
	PWM_CH_COUNT
};

enum fan_channel {
	FAN_CH_0 = 0,
	/* Number of FAN channels */
	FAN_CH_COUNT,
};

enum mft_channel {
	MFT_CH_0 = 0,
	/* Number of MFT channels */
	MFT_CH_COUNT,
};

enum temp_sensor_id {
	TEMP_SENSOR_1_CHARGER,
	TEMP_SENSOR_2_PP3300_REGULATOR,
	TEMP_SENSOR_3_DDR_SOC,
	TEMP_SENSOR_4_FAN,
	TEMP_SENSOR_COUNT
};

enum usbc_port {
	USBC_PORT_C0 = 0,
	USBC_PORT_C1,
	USBC_PORT_COUNT
};

enum sensor_id {
	LID_ACCEL = 0,
	CLEAR_ALS,
	RGB_ALS,
	VSYNC,
	SENSOR_COUNT,
};

/*
 * Daughterboard type is encoded in the lower 4 bits
 * of the FW_CONFIG CBI tag.
 */

enum usb_db_id {
	USB_DB_NONE = 0,
	USB_DB_USB4 = 1,
	USB_DB_USB3 = 2,
	USB_DB_COUNT
};

#define CBI_FW_CONFIG_USB_DB_MASK	0x0f
#define CBI_FW_CONFIG_USB_DB_SHIFT	0
#define CBI_FW_CONFIG_USB_DB_TYPE(bits) \
	(((bits) & CBI_FW_CONFIG_USB_DB_MASK) >> CBI_FW_CONFIG_USB_DB_SHIFT)

void board_reset_pd_mcu(void);

/* Common definition for the USB PD interrupt handlers. */
void ppc_interrupt(enum gpio_signal signal);
void tcpc_alert_event(enum gpio_signal signal);
void bc12_interrupt(enum gpio_signal signal);

unsigned char get_board_id(void);

#endif /* !__ASSEMBLER__ */

#endif /* __CROS_EC_BASEBOARD_H */
