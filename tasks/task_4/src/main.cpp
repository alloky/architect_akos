#include "Compiler.h"
#include "cpuemu.h"

#include <iostream>
#include <windows.h>

int main() {
	Compiler c;
	c.compile("test_prog", ".", "out_bins");
	CpuEmu emulator(20000);
	emulator.execute("out_bins/test_prog_0.0.1.asmbin");
}