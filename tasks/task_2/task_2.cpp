
#include "logging.h"
#include "filelineprocesser.h"



/**
 * @brief      Generating bread
 *
 * @param[in]  p_count  The p count
 *
 * @return     { description_of_the_return_value }
 */
FileLineProcesser::LinesView* generate_bread(size_t p_count){
	auto lv = new LinesView();
	char* empty_line = NULL;
	for(size_t i = 0; i < ptrs.size(); ++i){
		if(strlen(ptrs[i]) == 0){
			empty_line = ptrs[i];
			break; 
		}
	}
	
	bool contents_empty = true;
	if(empty_line == NULL){
		contents_empty = false;	
	}
	for (size_t i = 0; i < p_count; ++i){
		// generating paragraph
		for (size_t k = 0; k < 2; ++k){
			int r_line = 0, r_len = 0, nr_line = 0;
			do {
				r_line = rand() % ptrs.size();
				r_len = strlen(ptrs[r_line]);
			} while ( r_len == 0 );
			
			int r_shift = 0;
			while (true) {
				if (r_shift <= 0){
					r_shift *= -1;
					++r_shift;
				} else {
					r_shift *= -1;
				}
				if (r_line + r_shift >= (int)ptrs.size()){
					continue;
				}
				
				nr_line = r_line + r_shift;
				int nr_len = strlen(ptrs[nr_line]);
				bool flag = false;
				if (nr_len == 0) continue;
				for (int r_i = r_len - 1, nr_i = nr_len - 1; r_i >= 0 && nr_i >= 0;){
					if ((ptrs[r_line][r_i] == ' ') || (ptrs[nr_line][nr_i] == ' ')){
						flag = true;
						break;
					}
					if (_is_prep(ptrs[r_line][r_i])){
						--r_i;
						continue;
					}
					if (_is_prep(ptrs[nr_line][nr_i])){
						--nr_i;
						continue;
					}
					if(ptrs[r_line][r_i] != ptrs[nr_line][nr_i]){
						break;
					}
					--r_i;
					--nr_i;
				}
				if(!flag) break; 
			}
			//std::cout << ptrs[r_line] << '\n';
			//std::cout << ptrs[nr_line] << '\n';
			lv->ptrs.push_back(ptrs[r_line]);
			lv->ptrs.push_back(ptrs[nr_line]);
		}
		if(contents_empty){
			lv->ptrs.push_back(empty_line);
		}
		//std::cout << '\n';
	}
	return lv;
}

void sort(FileLineProcesser::LinesView* lv){
  std::sort(lv->lines.begin(), lv->lines.end(),
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


void sort_backwise(FileLineProcesser::LinesView* lv){
  std::sort(lv->lines.begin(), lv->lines.end(),
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

int main(){
	LOG("Programm to read files by lines");

	FileLineProcesser fr;
	int status = fr.read("data/onegin.txt");

	LEV_LOG(LL_DEBUG, "status : " << status);

	auto lv = fr.make_LinesView();

	LEV_LOG(LL_INFO, "Number of lines " << lv->ptrs.size());

	int print_count = 8;

	LEV_LOG(LL_INFO, "Alphabet sorting...");
	sort(lv);
	LEV_LOG(LL_INFO, "Done");
	lv->print_top_non_empty(print_count);
	lv->write("data/onegin_alph.txt");


	LEV_LOG(LL_INFO, "Backwise sorting...");
	sort_backwise(lv);
	lv->print_top_non_empty(print_count);
	lv->write("data/onegin_back.txt");


	LEV_LOG(LL_INFO, "Generating bread...");
	auto bread_lv = generate_bread(lv, print_count);
	bread_lv->print_top_non_empty(print_count);
	bread_lv->write("data/onegin_bread.txt");

	return 0;
}
