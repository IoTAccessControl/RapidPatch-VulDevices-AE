# Zephyr MQTT Subscriber App

board: NRF52840
host environment: x86-64, Ubuntu 19.10

## Build

```
export ZEPHYR_BASE="your zephyr base dir path"
export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr

cd $ZEPHYR_BASE

west build -p -b nrf52840_pca10056 IoTPatch/VulDevices/zephyr-app-mqtt -- -DOVERLAY_CONFIG=overlay-mqtt.conf
```

The build outputs are stored in `$ZEPHYR_BASE/build/` by default.


## Evaluation

We use mosquitto broker and mosquitto publisher on host machine to help evaluate the Zephyr MQTT Subscriber App on NRF52840 board.

To install mosquitto broker and client on host machine:

```
sudo apt-get update
sudo apt-get install mosquitto
sudo apt-get install mosquitto-clients
```

To run mosquitto broker on host machine:

```
sudo mosquitto -v -p 1883
```

To publish a single MQTT message on host machine:

```
sudo mosquitto_pub -t test_topic -m helloworld -p 1883
```

To batch publish MQTT messages on host machine:

```
cd IoTPatch/VulDevices/zephyr-app-mqtt
cd evaluation
python3 batch_mqtt_publish.py
```

We measure the duration between a PUBLISH message delivered by the broker and the corresponding PUBACK.

### Evaluation Steps

1. Build and flash Zephyr MQTT Subscriber App to NRF52840 board.

2. Connect the board and the host machine using Ethernet or WiFi.

3. Run mosquitto broker on host machine.

```
sudo mosquitto -v -p 1883
```

4. Press reset button on the NRF52840 board to rerun the MQTT Subscriber App.

5. Run the timing tool.

```
python3 publish_timing.py
```

6. Run batch publish tool.

```
python3 batch_mqtt_publish.py
```