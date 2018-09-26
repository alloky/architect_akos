
#include "logging.h"
#include "filelineprocesser.h"

int main(){
	LOG("Programm to read files by lines");

	FileLineProcesser fr;
	int status = fr.read("data/onegin.txt");

	LEV_LOG(LL_DEBUG, "status : " << status);

	auto lv = fr.make_LinesView();

	LEV_LOG(LL_INFO, "Number of lines " << lv->ptrs.size());

	int print_count = 8;

	LEV_LOG(LL_INFO, "Alphabet sorting...");
	lv->sort();
	LEV_LOG(LL_INFO, "Done");
	lv->print_top_non_empty(print_count);
	lv->write("data/onegin_alph.txt");


	LEV_LOG(LL_INFO, "Backwise sorting...");
	lv->sort_backwise();
	lv->print_top_non_empty(print_count);
	lv->write("data/onegin_back.txt");


	LEV_LOG(LL_INFO, "Generating bread...");
	auto bread_lv = lv->generate_bread(print_count);
	bread_lv->print_top_non_empty(print_count);
	bread_lv->write("data/onegin_bread.txt");

	return 0;
}
