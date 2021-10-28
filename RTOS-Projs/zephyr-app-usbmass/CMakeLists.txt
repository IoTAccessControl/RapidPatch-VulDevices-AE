# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

project(IoTPatch)

set(IOTPATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../PatchCore/)

message("======================================")
message(${IOTPATCH_DIR})
message("======================================")

file(GLOB iotpatchsrc
        "${IOTPATCH_DIR}/hotpatch/include/*.h"
        "${IOTPATCH_DIR}/hotpatch/src/*.c"
        "${IOTPATCH_DIR}/app/*.h"
        "${IOTPATCH_DIR}/app/*.c"
        "${IOTPATCH_DIR}/libebpf/include/*.h"
        "${IOTPATCH_DIR}/libebpf/src/*.c")

list(REMOVE_ITEM iotpatchsrc "${IOTPATCH_DIR}/libebpf/src/main.c")

include($ENV{ZEPHYR_BASE}/samples/net/common/common.cmake)

include_directories(
        "${IOTPATCH_DIR}/"
        "${IOTPATCH_DIR}/hotpatch"
        "${IOTPATCH_DIR}/app"
        "${IOTPATCH_DIR}/libebpf"
        "./src/usbmass"
        "$ENV{ZEPHYR_BASE}/subsys/net/ip"
)

add_definitions(-DSYS_CORTEX_M4 -DZEPHYR_OS -DDEV_NRF52840 -DDEV_USBMASS)

target_sources(app PRIVATE ${iotpatchsrc} src/main.c)