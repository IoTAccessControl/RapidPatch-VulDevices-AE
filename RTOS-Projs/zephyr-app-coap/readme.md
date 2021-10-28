
# Zephyr CoAP Server App

board: NRF52840
host environment: x86-64, Ubuntu 19.10

## Build

```
export ZEPHYR_BASE="your zephyr base dir path"
export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr

cd $ZEPHYR_BASE

west build -p -b nrf52840_pca10056 IoTPatch/VulDevices/zephyr-app-coap -- -DOVERLAY_CONFIG=overlay-coapperf.conf
```

The build outputs are stored in `$ZEPHYR_BASE/build/` by default.

## Evaluation

We use a CoAP client on the host machine to evaluate the performance of Zephyr CoAP Server App on NRF52840 board.

The CoAP client sends GET requests to the CoAP server and the request-response latencies are recorded in *out.txt*.

- Requests are sent sequentially.
- Latencies are recorded as microseconds.

```
cd IoTPatch/VulDevices/zephyr-app-coap
cd evaluation
./coap_client help
./coap_client -i 192.0.2.3 -n 10000
cat out.txt
```
