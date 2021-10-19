## Notes

### Debug  
use gdb:  
https://interrupt.memfault.com/blog/advanced-gdb?query=gdb  

#### Usage  
Set arm-none-eabi TOOLCHAIN:  
```
# cmake line 8: modify to your path
set(TOOLCHAIN_PREFIX /mnt/f/IoT/Tools/gcc-arm-none-eabi-9-2020-q2-update/)
```

Running...  
```
mkdir build
cd build && cmake ..
make qemu
```

18015 (178) -> 16377（175）
41k -> 37k

154 * 2 + 

14735 -> 13130

-Os
INM: bin:18k elf:141k  5166
ORI: bin:24k elf:146k  7918