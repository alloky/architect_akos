#include "Compiler.h"
#include "cpuemu.h"
#include "twitchLangCompiler.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

int main() {
	Compiler c;
	TwitchLangCompiler twlangc;


	std::ifstream ifs("twitch_lang.mdown");
	std::string twlangcode((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	twlangc.compile(twlangcode, "twitch_test_prog.txt");
	c.compile("twitch_test_prog", ".", "out_bins");
	
	CpuEmu emulator(20000);
	emulator.execute("out_bins/twitch_test_prog_0.0.1.asmbin");

}