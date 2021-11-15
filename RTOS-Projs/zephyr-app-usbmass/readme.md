
# Build

## NRF52840

```
west build -p -b nrf52840_pca10056 IoTPatch/VulDevices/zephyr-app-usbmass -- -DOVERLAY_CONFIG=overlay-usbmass.conf
```

# Zephyr MQTT Subscriber App

board: NRF52840
host environment: x86-64, Ubuntu 19.10

## Build

```
export ZEPHYR_BASE="your zephyr base dir path"
export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr

cd $ZEPHYR_BASE

west build -p -b nrf52840_pca10056 IoTPatch/VulDevices/zephyr-app-usbmass -- -DOVERLAY_CONFIG=overlay-usbmass.conf
```

The build outputs are stored in `$ZEPHYR_BASE/build/` by default.


## Evaluation

After flashing, we connect the board to the host PC using a USB cable and now the NRF52840 board would function as a USB Mass Storage device.

We use the script, `mass_USB_perf.py` to send USB commands to the board. In detail, for each iteration of the main loop, we send several read/write commands to the board, and measure the overall delays. 

### Evaluation Steps

1. Build and flash Zephyr USB Mass Storage App to NRF52840 board.

2. Connect the board and the host machine using a USB cable.

3. Run the USB mass storage evaluation script on host machine.

```
python3 mass_USB_perf.py
```

> You might have to install some dependecies in order to run this script.
