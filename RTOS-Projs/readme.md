
## Setup
mkdir IoTPatch  
将当前项目和PatchCore都放在IoTPatch目录下。  

TODO: 组织成submodule模式。

目录结构：  
IoTPatch  
 | PatchCore (git clone https://github.com/IoTAccessControl/PatchCore.git)  
 | VulDevices (git clone https://github.com/IoTAccessControl/VulDevices.git)  

## Build
zephyr源码修改：  
目前是直接手动修改，后面改成利用git patch工具去修改。  
由于官方的__debug_monitor没有加weak，因此链接时和我们自定义的函数冲突了，要注释掉官方的debug_monitor实现：  
修改zephyrproject\zephyr\arch\arm\core\fault_s.S,注释：  
``` bash
32行 # GTEXT(__debug_monitor)
72行 # SECTION_SUBSEC_FUNC(TEXT,__fault,__debug_monitor)
```

#### Network Build
echo server:  
west build -b stm32l475_my samples/net/sockets/echo_server --  -DOVERLAY_CONFIG=overlay-netusb.conf  

west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-coapperf.conf
west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-mqtt.conf
west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-netusb.conf  
west build -b stm32l475_my .  -- -DOVERLAY_CONFIG=overlay-netusb.conf  

west build -b 

Flash:  
nrfjprog -f nrf52 --program E:\PaperWork\IoT\OS\IoTPatch\VulDevices\zephyr-app\build\zephyr\zephyr.hex --sectorerase --reset  


#### USB Mass Build 
west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-usbmass.conf  
windwos上需要使用libusbK驱动  

windows下查看gbk错误信息：  
s.encode("latin1").decode("gbk")  

arm-none-eabi-objdump -S IoTPatch.axf > IoTPatch.list

arm-none-eabi-gdb

## NuttX Build
#### 配置NuttX源码

nrf52840设备：
./tools/configure.sh -l nrf52840-dk:nsh


#### Bin转成Hex
arm-none-eabi-objcopy -O ihex nuttx nuttx.hex

nrfjprog -f nrf52 --program F:\IoT\OS\nuttx\nuttx\nuttx.hex --sectorerase --reset  