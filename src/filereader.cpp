#include "filereader.h"

LinesView&& FileReader::make_LinesView(){
	LinesView lv;
	char** ptr_arr;
	int size = make_ptr_arr(buffer, ptr_arr);
	lv.ptrs = std::std::vector<char*>(ptr_arr);
	return std::move(lv);
}

int FileReader::open(const char* filename){
	return read_file(filename, buffer, &total_size);
}

FileReader::~FileReader(){
	if(buffer != NULL)
		delete buffer;
}