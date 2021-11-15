## Introduction

This directory contains the bare-metal firmware for the NRF52840-DK device and the STM32L475-iot-node device. You can use Keil Arm uVersion to open these two bare-metal projects.  

## Usage

To add the source code of RapidPatch Runtime to Keil template project:

```
python3 update_keil_proj.py STM32L475/USER ../../Runtime/stm32.yaml
python3 update_keil_proj.py NRF52840/project/mdk5 ../../Runtime/nrf52840.yaml
```

Then you need to compile and flash them to real devices.