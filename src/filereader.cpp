
#include "filereader.h"
#include "logging.h"



FileReader::LinesView* FileReader::make_LinesView(){
	LinesView* lv = new LinesView();
	char** ptr_arr = NULL;
	size_t lines_count = get_lines_count(buffer);
	size_t size = make_ptr_arr(buffer, &ptr_arr);
	lv->ptrs = std::vector<char*>(ptr_arr, ptr_arr + size);
	return lv;
}

int FileReader::open(const char* filename){
	if(buffer != NULL)
		delete buffer;
	int ret = read_file(filename, &buffer, &total_size);
	// LEV_LOG(LL_DEBUG, buffer);
	return ret;
}

void FileReader::LinesView::print_top_non_empty(size_t size){
  auto min_l = std::min((size_t)size, (size_t)ptrs.size());

  LEV_LOG(LL_INFO, "Top " << min_l << " non-empty lines");

  for (size_t i = 0, j = 0; i < min_l; ++j) {
  	if(ptrs[j][0] >= 'A' && ptrs[j][0] <= 'H') {
  		std::cout << ptrs[j] << "\n";
  		++i;
  	}
  }
}


void FileReader::LinesView::sort(){
  std::sort(ptrs.begin(), ptrs.end(),
    [] (const char* s1, const char* s2){
    if(s1[0] == '\0' && s2[0] == '\0'){
    	return false;
    }
    if(s1[0] == '\0'){
    	return false;
    }
    if(s2[0] == '\0'){
    	return true;
    }
   	for (size_t i = 0, j = 0; s1[i] != '\0' && s2[i] != '\0';) {
      if(_is_prep(s1[i])){
      	++i;
      	continue;
      }
      if(_is_prep(s2[j])){
      	++j;
      	continue;
      }
      if (s1[i] >= s2[j]) {
        return false;
      }
      ++i;
      ++j;
    }
    return true;
  });
}


void FileReader::LinesView::sort_backwise(){
  std::sort(ptrs.begin(), ptrs.end(),
    [] (const char* s1, const char* s2){
    size_t len_1 = strlen(s1);
    size_t len_2 = strlen(s2);
    if(len_1 == 0 && len_2 == 0){
    	return false;
    }
    if(len_1 == 0){
    	return false;
    }
    if(len_2 == 0){
    	return true;
    }
    for (size_t i = len_1 - 1, j = len_2 - 1; i >= 0 && j >= 0;) {
      if(_is_prep(s1[i])){
      	--i;
      	continue;
      }
      if(_is_prep(s2[j])){
      	--j;
      	continue;
      }
      if (s1[i] >= s2[j]) {
        return false;
      }
      --i;
      --j;
    }
    return true;
  });

}

bool FileReader::LinesView::_is_prep(char c){
	switch(c){
		case '.': case ';': case ',': case '!': case '?': 
			return true;
		default:
			return false;
	}
}

FileReader::FileReader() :
 buffer(NULL),
 total_size(0) {

 }

FileReader::~FileReader(){
	if(buffer != NULL)
		delete buffer;
}