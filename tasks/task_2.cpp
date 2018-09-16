#include <iostream>

#include "logging.h"
#include "filereader.h"

int main(){
	LOG("Programm to read files by lines");

	FileReader fr;
	int status = fr.open("data/text.txt");

	LEV_LOG(LL_DEBUG, "status : " << status);

	auto lv = fr.make_LinesView();
	for(auto t : lv.ptrs){
		LOG(t);
	}

	LOG("Number of lines ");
	LOG(lv.ptrs.size());
}