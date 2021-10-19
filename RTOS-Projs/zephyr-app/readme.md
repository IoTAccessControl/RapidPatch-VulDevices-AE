
## Build

#### Nrf52840
west build -b nrf52840_PCA10056 . 

Flash:  
nrfjprog -f nrf52 --program E:\PaperWork\IoT\OS\IoTPatch\VulDevices\zephyr-app\build\zephyr\zephyr.hex --sectorerase --reset  

west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-coapperf.conf
west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-mqtt.conf
west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-netusb.conf  


#### stm32L475
west build -b stm32l475_my .  
st-flash --reset write E:\PaperWork\IoT\OS\IoTPatch\VulDevices\zephyr-app\build\zephyr\zephyr.bin 0x08000000   
需要擦除后，才能用keil继续刷：st-flash erase  
stlink工具：https://github.com/stlink-org/stlink（最新版windows下有bug，需要自己编译）  

west build -b stm32l475_my .  -- -DOVERLAY_CONFIG=overlay-netusb.conf    

#### stm32F429
west build -b stm32f429i_disc1 .
st-flash --reset write E:\PaperWork\IoT\OS\IoTPatch\VulDevices\zephyr-app\build\zephyr\zephyr.bin 0x08000000   



## 不同Demo

#### USB Mass Build 
west build -b nrf52840_PCA10056 .  -- -DOVERLAY_CONFIG=overlay-mqtt.conf


## 全局Patch
Storage:
CONFIG_STACK_CANARIES:
stack_canary bin 193 -> 238

FINSTRUCT:
193 -> 260
看list文件除了增加的行，还增加了3w行。说明编译优化出了问题： 
137895 - 2375 * 8

hex: 542k

106184: 1993 -> (229k)
多了300个函数？？
函数个数换算之后：
    Func | Line | Size  
INS：2375 | 137895 | 
ORI: 1992 | 106184 | 193k
换算 193k -> 230k

优化大小：
set(OPTIMIZATION_FLAG -Os)
zephyr_compile_options(${OPTIMIZATION_FLAG})

需要配置合理的：
-finstrument-functions-exclude-function-list


不删除未使用函数：
2880 342k
2763 254k -> 264k


###  ZephyrOS的修改  

修改zephyrproject\zephyr\arch\arm\core\fault_s.S,注释：  
``` bash
32行 # GTEXT(__debug_monitor)
72行 # SECTION_SUBSEC_FUNC(TEXT,__fault,__debug_monitor)
```
# 手动修改
- SECTION_SUBSEC_FUNC(TEXT,__fault,__debug_monitor)
+ # SECTION_SUBSEC_FUNC(TEXT,__fault,__debug_monitor)