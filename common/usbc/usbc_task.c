/* Copyright 2019 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "battery.h"
#include "battery_smart.h"
#include "board.h"
#include "charge_manager.h"
#include "charge_state.h"
#include "chipset.h"
#include "common.h"
#include "console.h"
#include "ec_commands.h"
#include "gpio.h"
#include "hooks.h"
#include "host_command.h"
#include "registers.h"
#include "system.h"
#include "task.h"
#include "timer.h"
#include "util.h"
#include "usb_charge.h"
#include "usb_mux.h"
#include "usb_pd.h"
#include "usb_prl_sm.h"
#include "tcpm.h"
#include "usb_pe_sm.h"
#include "usb_prl_sm.h"
#include "usb_sm.h"
#include "usb_tc_sm.h"
#include "usbc_ppc.h"
#include "version.h"

#define USBC_EVENT_TIMEOUT (5 * MSEC)

static uint8_t paused[CONFIG_USB_PD_PORT_MAX_COUNT];

int tc_restart_tcpc(int port)
{
	return tcpm_init(port);
}

void tc_pause_event_loop(int port)
{
	paused[port] = 1;
}

void tc_start_event_loop(int port)
{
	paused[port] = 0;
	task_set_event(PD_PORT_TO_TASK_ID(port), TASK_EVENT_WAKE, 0);
}

/* High-priority interrupt tasks implementations */
#if     defined(HAS_TASK_PD_INT_C0) || defined(HAS_TASK_PD_INT_C1) || \
	defined(HAS_TASK_PD_INT_C2)

/* Used to conditionally compile code in main pd task. */
#define HAS_DEFFERED_INTERRUPT_HANDLER

/* Events for pd_interrupt_handler_task */
#define PD_PROCESS_INTERRUPT  (1<<0)

static uint8_t pd_int_task_id[CONFIG_USB_PD_PORT_MAX_COUNT];

void schedule_deferred_pd_interrupt(const int port)
{
	task_set_event(pd_int_task_id[port], PD_PROCESS_INTERRUPT, 0);
}

/*
 * Main task entry point that handles PD interrupts for a single port
 *
 * @param p The PD port number for which to handle interrupts (pointer is
 * reinterpreted as an integer directly).
 */
void pd_interrupt_handler_task(void *p)
{
	const int port = (int) p;
	const int port_mask = (PD_STATUS_TCPC_ALERT_0 << port);

	ASSERT(port >= 0 && port < CONFIG_USB_PD_PORT_MAX_COUNT);

	pd_int_task_id[port] = task_get_current();

	while (1) {
		const int evt = task_wait_event(-1);

		if (evt & PD_PROCESS_INTERRUPT) {
			/*
			 * While the interrupt signal is asserted; we have more
			 * work to do. This effectively makes the interrupt a
			 * level-interrupt instead of an edge-interrupt without
			 * having to enable/disable a real level-interrupt in
			 * multiple locations.
			 *
			 * Also, if the port is disabled do not process
			 * interrupts. Upon existing suspend, we schedule a
			 * PD_PROCESS_INTERRUPT to check if we missed anything.
			 */
			while ((tcpc_get_alert_status() & port_mask) &&
					pd_is_port_enabled(port))
				tcpc_alert(port);
		}
	}
}
#endif /* HAS_TASK_PD_INT_C0 || HAS_TASK_PD_INT_C1 || HAS_TASK_PD_INT_C2 */

void pd_task(void *u)
{
	int port = TASK_ID_TO_PD_PORT(task_get_current());

	tc_state_init(port);

	if (IS_ENABLED(CONFIG_USBC_PPC))
		ppc_init(port);

	/*
	 * Since most boards configure the TCPC interrupt as edge
	 * and it is possible that the interrupt line was asserted between init
	 * and calling set_state, we need to process any pending interrupts now.
	 * Otherwise future interrupts will never fire because another edge
	 * never happens. Note this needs to happen after set_state() is called.
	 */
	if (IS_ENABLED(HAS_DEFFERED_INTERRUPT_HANDLER))
		schedule_deferred_pd_interrupt(port);


	while (1) {
		/* wait for next event/packet or timeout expiration */
		const uint32_t evt =
			task_wait_event(paused[port]
						? -1
						: USBC_EVENT_TIMEOUT);

		/* handle events that affect the state machine as a whole */
		tc_event_check(port, evt);

		/*
		 * run port controller task to check CC and/or read incoming
		 * messages
		 */
		if (IS_ENABLED(CONFIG_USB_PD_TCPC))
			tcpc_run(port, evt);

		if (IS_ENABLED(CONFIG_USB_PE_SM))
			/* Run policy engine state machine */
			pe_run(port, evt, tc_get_pd_enabled(port));

		if (IS_ENABLED(CONFIG_USB_PRL_SM))
			/* Run protocol state machine */
			prl_run(port, evt, tc_get_pd_enabled(port));

		/* Run TypeC state machine */
		tc_run(port);
	}
}
