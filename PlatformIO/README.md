We plan to release RapidPatch Runtime as a PlatformIO library.   

However, at this time, you still need to create  the PlatformIO  projects manually. You need to choose a device with Cortex-M3/M4 MCUs and  create an empty project. Then you need to copy the RapidPatch Runtime code into the library directory.  This approach is trivial as RapidPatch need to modify the origin RTOSs which have defined a DebugMonitor Handler, e.g., Zephyr (the patch file is in RTOS-proj/zephyr-app/zephy_diff.patch). 

If you have real-devices and want to run the RapidPatch on this devices, please contact me (heyi21@mails.tsinghua.edu.cn or clangllvm@126.com) and I will create a PlatformIO demo project for you.



