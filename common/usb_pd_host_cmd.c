/* Copyright 2020 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 *
 * Host commands for USB-PD module.
 */

#include <string.h>

#include "ec_commands.h"
#include "host_command.h"
#include "tcpm.h"
#include "usb_pd.h"
#include "usb_pd_tcpm.h"

#ifdef CONFIG_COMMON_RUNTIME
struct ec_params_usb_pd_rw_hash_entry rw_hash_table[RW_HASH_ENTRIES];
#endif /* CONFIG_COMMON_RUNTIME */

#ifdef HAS_TASK_HOSTCMD

static enum ec_status hc_pd_ports(struct host_cmd_handler_args *args)
{
	struct ec_response_usb_pd_ports *r = args->response;

	r->num_ports = board_get_usb_pd_port_count();
	args->response_size = sizeof(*r);

	return EC_RES_SUCCESS;
}
DECLARE_HOST_COMMAND(EC_CMD_USB_PD_PORTS,
		     hc_pd_ports,
		     EC_VER_MASK(0));

#ifdef CONFIG_HOSTCMD_RWHASHPD
static enum ec_status
hc_remote_rw_hash_entry(struct host_cmd_handler_args *args)
{
	int i, idx = 0, found = 0;
	const struct ec_params_usb_pd_rw_hash_entry *p = args->params;
	static int rw_hash_next_idx;

	if (!p->dev_id)
		return EC_RES_INVALID_PARAM;

	for (i = 0; i < RW_HASH_ENTRIES; i++) {
		if (p->dev_id == rw_hash_table[i].dev_id) {
			idx = i;
			found = 1;
			break;
		}
	}

	if (!found) {
		idx = rw_hash_next_idx;
		rw_hash_next_idx = rw_hash_next_idx + 1;
		if (rw_hash_next_idx == RW_HASH_ENTRIES)
			rw_hash_next_idx = 0;
	}
	memcpy(&rw_hash_table[idx], p, sizeof(*p));

	return EC_RES_SUCCESS;
}
DECLARE_HOST_COMMAND(EC_CMD_USB_PD_RW_HASH_ENTRY,
		     hc_remote_rw_hash_entry,
		     EC_VER_MASK(0));
#endif /* CONFIG_HOSTCMD_RWHASHPD */

#ifndef CONFIG_USB_PD_TCPC
#ifdef CONFIG_EC_CMD_PD_CHIP_INFO
static enum ec_status hc_remote_pd_chip_info(struct host_cmd_handler_args *args)
{
	const struct ec_params_pd_chip_info *p = args->params;
	struct ec_response_pd_chip_info_v1 *info;

	if (p->port >= board_get_usb_pd_port_count())
		return EC_RES_INVALID_PARAM;

	if (tcpm_get_chip_info(p->port, p->live, &info))
		return EC_RES_ERROR;

	/*
	 * Take advantage of the fact that v0 and v1 structs have the
	 * same layout for v0 data. (v1 just appends data)
	 */
	args->response_size =
		args->version ? sizeof(struct ec_response_pd_chip_info_v1)
			      : sizeof(struct ec_response_pd_chip_info);

	memcpy(args->response, info, args->response_size);

	return EC_RES_SUCCESS;
}
DECLARE_HOST_COMMAND(EC_CMD_PD_CHIP_INFO,
		     hc_remote_pd_chip_info,
		     EC_VER_MASK(0) | EC_VER_MASK(1));
#endif /* CONFIG_EC_CMD_PD_CHIP_INFO */
#endif /* CONFIG_USB_PD_TCPC */

#endif /* HAS_TASK_HOSTCMD */