# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

project(IoTPatch)

set(IOTPATCH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../RapidPatch-Runtime-AE/)

message("======================================")
message("PatchCore:" ${IOTPATCH_DIR} "  PROJ:" ${PROJECT_SOURCE_DIR})
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
        "${PROJECT_SOURCE_DIR}/src/mqtt"
        "${PROJECT_SOURCE_DIR}/src/coap"
        "$ENV{ZEPHYR_BASE}/subsys/net/ip"
)

# add_definitions(-DSYS_CORTEX_M4 -DZEPHYR_OS)
# add_definitions(-DSYS_CORTEX_M4 -DZEPHYR_OS -DDEV_STM32F429)
# add_definitions(-DSYS_CORTEX_M4 -DZEPHYR_OS -DDEV_STM32F429 -DDEV_MQTT)

# target_compile_options(app PRIVATE -finstrument-functions) 
# 只加单个目录
# add_compile_options(-finstrument-functions)

# target_sources(app PRIVATE src/main_empty.c src/coap/coap-server.c)
# target_sources(app PRIVATE ${iotpatchsrc} src/main.c)


# coap
# add_definitions(-DSYS_CORTEX_M4 -DZEPHYR_OS -DDEV_NRF52840 -DDEV_COAP)
# target_sources(app PRIVATE ${iotpatchsrc} src/main.c src/coap/coap-server.c)


# mqtt
# add_definitions(-DSYS_CORTEX_M4 -DZEPHYR_OS -DDEV_NRF52840 -DDEV_MQTT)
# target_sources(app PRIVATE ${iotpatchsrc} src/main.c src/mqtt/mqtt-subscriber.c)

# basic system and usb
target_sources(app PRIVATE ${iotpatchsrc} src/main.c)
add_definitions(-DSYS_CORTEX_M4 -DZEPHYR_OS -DDEV_NRF52840)