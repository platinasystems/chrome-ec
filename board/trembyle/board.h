/* Copyright 2019 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/* Trembyle board configuration */

#ifndef __CROS_EC_BOARD_H
#define __CROS_EC_BOARD_H

#define VARIANT_ZORK_TREMBYLE

#include "baseboard.h"

/*
 * Allow dangerous commands.
 * TODO: Remove this config before production.
 */
#define CONFIG_SYSTEM_UNLOCKED
#define CONFIG_I2C_DEBUG

#define CONFIG_MKBP_USE_GPIO

/* Motion sensing drivers */
#define CONFIG_ACCELGYRO_BMI160
#define CONFIG_ACCELGYRO_BMI160_INT_EVENT \
	TASK_EVENT_MOTION_SENSOR_INTERRUPT(BASE_ACCEL)
#define CONFIG_ACCEL_INTERRUPTS
#define CONFIG_ACCEL_KX022
#define CONFIG_CMD_ACCELS
#define CONFIG_CMD_ACCEL_INFO
#define CONFIG_TABLET_MODE
#define CONFIG_LID_ANGLE
#define CONFIG_LID_ANGLE_UPDATE
#define CONFIG_LID_ANGLE_SENSOR_BASE BASE_ACCEL
#define CONFIG_LID_ANGLE_SENSOR_LID LID_ACCEL

/* GPIO mapping from board specific name to EC common name. */
#define CONFIG_BATTERY_PRESENT_GPIO	GPIO_EC_BATT_PRES_ODL
#define GPIO_AC_PRESENT			GPIO_ACOK_OD
#define GPIO_CPU_PROCHOT		GPIO_PROCHOT_ODL
#define GPIO_EC_INT_L			GPIO_EC_AP_INT_ODL
#define GPIO_ENABLE_BACKLIGHT_L		GPIO_EC_EDP_BL_DISABLE
#define GPIO_ENTERING_RW		GPIO_EC_ENTERING_RW
#define GPIO_KBD_KSO2			GPIO_EC_KSO_02_INV
#define GPIO_PCH_RSMRST_L		GPIO_EC_FCH_RSMRST_L
#define GPIO_PCH_SLP_S3_L		GPIO_SLP_S3_L
#define GPIO_PCH_SLP_S5_L		GPIO_SLP_S5_L
#define GPIO_PCH_WAKE_L			GPIO_EC_FCH_WAKE_L
#define GPIO_POWER_BUTTON_L		GPIO_EC_PWR_BTN_ODL
#define GPIO_S0_PGOOD			GPIO_S0_PWROK_OD
#define GPIO_S5_PGOOD			GPIO_EC_PWROK_OD
#define GPIO_SYS_RESET_L		GPIO_EC_SYS_RST_L
#define GPIO_VOLUME_DOWN_L		GPIO_VOLDN_BTN_ODL
#define GPIO_VOLUME_UP_L		GPIO_VOLUP_BTN_ODL
#define GPIO_WP_L			GPIO_EC_WP_L

#ifndef __ASSEMBLER__

/* These GPIOs moved. Temporarily detect and support the V0 HW. */
extern enum gpio_signal GPIO_PCH_PWRBTN_L;
extern enum gpio_signal GPIO_PCH_SYS_PWROK;

enum battery_type {
	BATTERY_AP18F4M,
	BATTERY_TYPE_COUNT,
};

enum mft_channel {
	MFT_CH_0 = 0,
	/* Number of MFT channels */
	MFT_CH_COUNT,
};

enum pwm_channel {
	PWM_CH_KBLIGHT = 0,
	PWM_CH_FAN,
	PWM_CH_COUNT
};

#endif /* !__ASSEMBLER__ */


#endif /* __CROS_EC_BOARD_H */
