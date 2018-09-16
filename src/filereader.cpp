#include "filereader.h"
#include "logging.h"

FileReader::LinesView&& FileReader::make_LinesView(){
	LinesView lv;
	char** ptr_arr = NULL;
	size_t lines_count = get_lines_count(buffer);
	lv.ptrs = std::vector<char*>(lines_count);
	size_t size = make_ptr_arr(buffer, &(lv.ptrs[0]));
	LEV_LOG(LL_DEBUG, "size of ptr_arr : " << size);
	return std::move(lv);
}

int FileReader::open(const char* filename){
	int ret = read_file(filename, &buffer, &total_size);
	LOG(buffer);
	return ret;
}

FileReader::~FileReader(){
	if(buffer != NULL)
		delete buffer;
}