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

#ifdef DEV_COAP
#include "common.h"
#else
#define STACK_SIZE 1024
#define THREAD_PRIORITY K_PRIO_COOP(8)
#endif

#ifdef DEV_MQTT
#include "config.h"
#endif




K_THREAD_DEFINE(cli_thread, STACK_SIZE,
	run_shell_cli, NULL, NULL, NULL,
	THREAD_PRIORITY, 0, K_FOREVER);

static void load_fixed_patch_0(void);
static void ebpf_test();
extern void arm_core_mpu_disable();
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
	printk("Hello World! %s v:%f\n", CONFIG_BOARD, 1.45);
	k_thread_start(cli_thread);

	//run_coap_server();
	// profile_add_event("micro profile dynamic start");
	// profile_add_event("micro profile fixed start");
	// profile_add_event("uart print profile start");
	// profile_add_event("eBPF exec time evaluation start");

	#ifdef DEV_COAP
	// run_coap_server();
	#endif

	#ifdef DEV_MQTT
	run_mqtt_subscriber();
	#endif



	// ebpf_test();
}

#define __NAKE __attribute__((naked))
#define __NOIST __attribute__((no_instrument_function))

// void __attribute__((no_instrument_function, naked)) 
// restore_context() {
// 	__asm__ __volatile__("BX lr");
// }

// void __attribute__((no_instrument_function))
// _patch_dispatch(uint32_t *ctx) {
// 	printf("_patch_dispatch: %p\n", ctx);
// 	// restore_context();
// 	printf("after _patch_dispatch\n");
// }

// void __attribute__((no_instrument_function, naked))
// __cyg_profile_func_enter(void *this_func, void *call_site) {
// 	//__asm("push {r0-r3, r12, lr}");
// 	//print_str("enter func\n");
// 	__asm__ __volatile__("PUSH {lr}");
// 	__asm__ __volatile__("TST lr, #4");
// 	__asm__ __volatile__("ITE EQ"); 
// 	__asm__ __volatile__("MRSEQ r0, MSP"); 
// 	__asm__ __volatile__("MRSNE r0, PSP"); 
// 	__asm__ __volatile__("B _patch_dispatch"); 
// 	__asm__ __volatile__("POP {lr}");
// 	__asm__ __volatile__("BX lr");
// }

// void __attribute__((no_instrument_function))
// __cyg_profile_func_exit(void *this_fn, void *call_site) {
// 	printf("exit func\n");
// }
