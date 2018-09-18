
#include "filelineprocesser.h"
#include "logging.h"



FileLineProcesser::LinesView* FileLineProcesser::make_LinesView(){
	LinesView* lv = new LinesView();
	char** ptr_arr = NULL;
	size_t lines_count = get_lines_count(buffer);
	size_t size = make_ptr_arr(buffer, &ptr_arr);
	lv->ptrs = std::vector<char*>(ptr_arr, ptr_arr + size);
	return lv;
}

int FileLineProcesser::LinesView::write(const char* filename){
	/*
		Alphabet sorting and writting to file
	*/	
	LEV_LOG(LL_INFO, "Writitng file to output...");
	int ret = multi_write(filename, &(ptrs[0]), ptrs.size());
	LEV_LOG(LL_INFO, "Done");
	return ret;
}



int FileLineProcesser::read(const char* filename){
	if(buffer != NULL)
		delete buffer;
	int ret = read_file(filename, &buffer, &total_size);
	// LEV_LOG(LL_DEBUG, buffer);
	return ret;
}

int FileLineProcesser::write(const char* filename){
	if(buffer == NULL){
		return -1;
	}
	return write_to_file(filename, buffer, total_size);
}

void FileLineProcesser::LinesView::print_top_non_empty(size_t size){
	auto min_l = std::min((size_t)size, (size_t)ptrs.size());

	LEV_LOG(LL_INFO, "Top " << min_l << " non-empty lines\n");

	for (size_t i = 0, j = 0; i < min_l; ++j) {
		if(strlen(ptrs[j]) >= 10) {
			std::cout << ptrs[j] << "\n";
			++i;
		}
	}

	LEV_LOG(LL_INFO, "End\n");
}


void FileLineProcesser::LinesView::sort(){
  std::sort(ptrs.begin(), ptrs.end(),
    [] (const char* s1, const char* s2){
    if(s1[0] == '\0' && s2[0] == '\0'){
    	return false;
    }
    if(s1[0] == '\0'){
    	return true;
    }
    if(s2[0] == '\0'){
    	return false;
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
      if (s1[i] > s2[j]) {
        return false;
      }
      if(s1[i] < s2[j]){
      	return true;
      }
      ++i;
      ++j;
    }
    return false;
  });
}


void FileLineProcesser::LinesView::sort_backwise(){
  std::sort(ptrs.begin(), ptrs.end(),
    [] (const char* s1, const char* s2){
    int len_1 = strlen(s1);
    int len_2 = strlen(s2);
    for (int i = len_1 - 1, j = len_2 - 1; i >= 0 && j >= 0;) {
      if(_is_prep(s1[i])){
      	--i;
      	continue;
      }
      if(_is_prep(s2[j])){
      	--j;
      	continue;
      }
      if (s1[i] > s2[j]) {
        return false;
      }
      if(s1[i] < s2[j]){
      	return true;
      }
      --i;
      --j;
    }
    return false;
  });

}

bool FileLineProcesser::LinesView::_is_prep(char c){
	switch(c){
		case '.': case ';': case ',': case '!': case '?':
		case '[' : case ']' : case '(' : case ')' :
		case '"' : case ' ' : case '\'' :
		case '-' :
		case '0' : case '1' : case '2' :
		case '3' : case '4' : case '5' :
		case '6' : case '7' : case '9' :
			return true;
		default:
			return false;
	}
}

FileLineProcesser::FileLineProcesser() :
 buffer(NULL),
 total_size(0) {

 }

FileLineProcesser::~FileLineProcesser(){
	if(buffer != NULL)
		delete buffer;
}