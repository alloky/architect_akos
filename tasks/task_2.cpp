#include <iostream>

#include "logging.h"
#include "filereader.h"

int main(){
	LOG("Programm to read files by lines");

	FileReader fr;
	int status = fr.open("text.txt");

	auto lv = fr.make_LineView();
	LOG("Number of lines " << lv.ptrs.size());
}