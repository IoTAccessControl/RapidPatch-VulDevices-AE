## Introduction

This repository contains the demo projects used in our paper and you can compile and run these projects to validate the functionable of RapidPatch.   

```
-------------\
| - evaluation, The evluation and figure drawing scripts.
| - Keil-Baremetal-Projs, Bare-metal projects for NRF52840 and STM32L475
| - PlatformIO, 
| - qemu-stm32-p103 project, 
| - RTOS-Projs, Demo projects for Zephyr OS.
```

If you want to compile the RapidPatch Runtime, you can use one of these projects. Unfortunately, we cannot provide detail guide for you to configure the compiling environment for each RTOSs. To make it easier, we suggest you to directly use the IDE such as Keil or Platform-IO. Thus, you do not need to deal with the Makefiles.  

## Usage

If you use Windows and have a NRF52840 or STM32L475 devices, you can use Keil to open the projects.   

If you use Linux/Mac or you have other devices with Cortex-M3/M4, you can use Platform-IO.  

If you have experiments with ZephyrOS, you can directly use the zephyr-app project(in RTOS-Projs directory). 
