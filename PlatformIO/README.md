We plan to release RapidPatch Runtime as a PlatformIO library.   

However, at this time, you still need to create  the PlatformIO  projects manually. You need to choose a device with Cortex-M3/M4 MCUs and  create an empty project. Then you need to copy the RapidPatch Runtime code into the library directory.  This approach is trivial as RapidPatch need to modify the origin RTOSs which have defined a DebugMonitor Handler, e.g., Zephyr (the patch file is in RTOS-proj/zephyr-app/zephy_diff.patch). 

If you have real-devices and want to run the RapidPatch on this devices, please contact me (heyi21@mails.tsinghua.edu.cn or clangllvm@126.com) and I will create a PlatformIO demo project for you.



Here is a sample PlatformIO project for STM32F429 and STM32F407 with libopencm3.

### Run the Project

1. Use vscode Platform-IO to open the folder (VulDevices\PlatformIO\stm32f4xx)   
2. Wait for the vscode to install the toolchains and libopencm3 (may need several minutes)  

```
# if the tools are not installed, it will give you an error message when click build  
command 'platformio-ide.build' not found
```

3. Run the test commends

```

# use FPB dynamic patch points
$ run 0
run cmd: 0 {Test FPB patch trigger}
test_fpb_patch
trigger_fpb_patch
Call buggy func before fpb patch!
enter RawBuggyFunc
exit RawBuggyFunc
buggy_addr: 0x0800210c fixed_addr: 0x08002100
ADDR: 0x200011c0 excepted value: 0xbff8f7ff
remap: 0x200011c0 inst addr: 0xbff8f7ff
Call buggy func after fpb patch!
run FixedBuggyFunc

# use DebugMonitor, installing patch 0
$ run 1
run test_func: 0x08002895
Event 0 -> cycle: 40 time(us): 0
debug_monitor_handler_c lr:0x08000485 pc:0x08002894
run_ebpf_filter res: 1 0
filter and return: 0 0x08000485 0x08002894
set_return: 0x08000485
Event 0 -> cycle: 2143295 time(us): 21432
[0000] is bug fixed? yes
```

If you want to get the accurate execution time, you need to disable the TEST Logs by undefine the USE_TEST_LOG marco (in file lib\hotpatch\src\defs.h).



Notes that, if you get the follow error, you should power off your device after flash using PlatformIO to disable the gdb debug mode.

```
Halting Debug Enabled - Can't Enable Monitor Mode Debug!
**WARNING**: Please power off you devices and retry!
```

