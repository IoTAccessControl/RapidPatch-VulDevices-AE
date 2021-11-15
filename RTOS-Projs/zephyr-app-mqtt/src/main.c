/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <misc/printk.h>
#include <console.h>
#include <stdio.h>
#include <string.h>

#include "include/patch_service.h"
#include "include/iotpatch.h"
#include "libebpf/include/ebpf_allocator.h"
#include "include/utils.h"

#include "hotpatch/include/profiling.h"

#include "app/ihp_cli.h"

#ifdef DEV_MQTT
#include "config.h"
#endif


K_THREAD_DEFINE(cli_thread, STACK_SIZE,
	run_shell_cli, NULL, NULL, NULL,
	THREAD_PRIORITY, 0, K_FOREVER);

static void load_fixed_patch_0(void);
static void ebpf_test();
// extern void arm_core_mpu_disable();
void main(void)
{
	#define USE_JIT
	#ifdef USE_JIT
	// disable mpu
	arm_core_mpu_disable();
	// https://lists.zephyrproject.org/g/users/topic/random_fault_exception/30793151?p=
	// CONFIG_NO_OPTIMIZATIONS=y
	#endif

	console_init();
	//printk("Hello World! %s\n", CONFIG_BOARD);
	k_thread_start(cli_thread);

	//run_coap_server();
	profile_add_event("micro profile dynamic start");
	profile_add_event("micro profile fixed start");
	profile_add_event("uart print profile start");
	profile_add_event("eBPF exec time evaluation start");

	#ifdef DEV_MQTT
	run_mqtt_subscriber();
	#endif
}
