
#include "logging.h"
#include "filereader.h"

int main(){
	LOG("Programm to read files by lines");

	FileReader fr;
	int status = fr.open("data/onegin.txt");

	LEV_LOG(LL_DEBUG, "status : " << status);

	auto lv = fr.make_LinesView();

	LOG("Number of lines ");
	LOG(lv->ptrs.size());

	lv->sort();
	lv->sort_backwise();
	lv->print_top_non_empty(20);

	return 0;
}