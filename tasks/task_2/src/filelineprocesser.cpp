
#include "filelineprocesser.h"
#include "logging.h"



FileLineProcesser::LinesView* FileLineProcesser::make_LinesView(){
	LinesView* lv = new LinesView();
	
	if(precalc_ptrs == NULL){
		char** ptr_arr = NULL;
		size_t lines_count = get_lines_count(buffer);
		size_t size = make_ptr_arr(buffer, &ptr_arr);
		lv->lines = std::vector<string_view>(size);	
		for(size_t i = 0; i < size; ++i){
			lv->lines[i] = string_view(ptr_arr[i], strlen(ptr_arr[i]));
		}
	}

	*lv = *precalc_ptrs;

	return lv;
}

int FileLineProcesser::LinesView::write(const string& path, char sep='\n'){
	LEV_LOG(LL_INFO, "Writitng lines to file : " << path);

	SmartDescriptor temp_fd;	
	int status = temp_fd.open(path);
	if(status != 0){
		return status;
	}
	
	for(auto& line : lines){
		if((status = temp_fd.write(line, sep)) != 0){
			return status;
		}
	}
	temp_fd.close();

	LEV_LOG(LL_INFO, "Done");
	return ret;
}

int FileLineProcesser::open(const char* path){

	return sfd.open(path);
}

int FileLineProcesser::read(){
	if(data.buff != NULL)
		delete data.buff;
	return sfd.read_all(filename, &data.buff, &data.size);
}


void FileLineProcesser::LinesView::print_top_non_empty(size_t size){
	auto min_l = std::min((size_t)size, (size_t)ptrs.size());

	LEV_LOG(LL_INFO, "Top " << min_l << " non-empty lines\n");

	for (size_t i = 1, j = 0; i <= min_l; ++j) {
		if(strlen(ptrs[j]) >= 10) {
			std::cout << ptrs[j] << "\n";
			//if(i % 4 == 0) std::cout << "\n";
			++i;
		}
	}

	LEV_LOG(LL_INFO, "End\n");
}

bool FileLineProcesser::LinesView::_is_prep(char c){
	switch(c){
		case '.': case ';': case ',': case '!': case '?':
		case '[' : case ']' : case '(' : case ')' :
		case '"' : case ' ' : case '\'' :
		case '-' : case '*' :
		case '0' : case '1' : case '2' :
		case '3' : case '4' : case '5' :
		case '6' : case '7' : case '8' : case '9' :
			return true;
		default:
			return false;
	}
}

FileLineProcesser::FileLineProcesser() :
 buffer(NULL),
 total_size(0),
 precalc_ptrs(NULL) {

 }

FileLineProcesser::~FileLineProcesser(){
	if(buffer != NULL)
		delete buffer;
}
