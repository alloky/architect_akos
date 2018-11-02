# Code structure:

### Command system

All command are defined in **cpuemu-cmd-dfs.h**. In **Compiler.cpp** and **cpuemu.cpp** special defiens are used to convert arguments into working code.

### Compiling

Asm code is used from **CpuEmulator/CpuEmulator/test_prog.txt** and placed in **CpuEmulator/CpuEmulator/out_bins/test_prog_{$version}.asmbin** 

### Exectution

**CpuEmu** executes raw byte-code loaded from .asmbin  
