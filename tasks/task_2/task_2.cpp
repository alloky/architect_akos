
#include "logging.h"
#include "filelineprocesser.h"



/**
 * @brief      Generating bread
 *
 * @param[in]  p_count  The p count
 *
 * @return     { description_of_the_return_value }
 */
FileLineProcesser::LinesView* generate_bread(FileLineProcesser::LinesView& text, size_t p_count, int seed=42){
	LinesView* lv = new LinesView();
	for (size_t i = 0; i < p_count; ++i){
		// generating paragraph
		int paragraph[4];
		generate_paragraph(text, paragraph, seed);
		for(size_t j = 0; j < 4; ++j){
			lv.lines.push_back(text.lines[paragraph[j]]);
		}
		lv->lines.push_back(string_view(""));	
	}
	return lv;
}

void generate_paragraph(FileLineProcesser::LinesView& text, int* paragraph, int seed=42){
	srand(seed);
	for (size_t k = 0; k < 2; ++k){
		int first_line = 0, 
			second_line = 0;
		
		do {
			first_line = rand() % text.lines.size();;
		} while ( text[first_line].size == 0 );
		
		int second_line = find_rythm(first_line, text);
		paragraph[2*k] = first_line;
		paragraph[2*k+1] = second_line; 
	}
}

int find_rythm(int first_line_ind, FileLineProcesser::LinesView& text){
	int r_shift = 0;
	while (true) {
		int second_line_ind = choose_nearest(first_line_ind, shift, text.lines.size());
		if(!is_rythmed(text, first_line_ind, second_line_ind)){
			continue;
		}	
		return second_line_ind;
	}
}

bool is_rythmed(FileLineProcesser::LinesView& text, int first_line_ind, int second_line_ind){
	int nr_len = strlen(ptrs[nr_line]);
	bool flag = true;
	if (nr_len == 0) continue;
	for (int r_i = r_len - 1, nr_i = nr_len - 1; r_i >= 0 && nr_i >= 0;){
		if ((ptrs[r_line][r_i] == ' ') || (ptrs[nr_line][nr_i] == ' ')){
			flag = false;
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
	return flag;
}

int choose_nearest(int first_line, int& shift, size_t size){
	while(true){
		if (shift <= 0){
			shift *= -1;
			++shift;
		} else {
			shift *= -1;
		}
		if (first_line + shift >= (int)size){
			continue;
		}
		return first_line + shift;
	}
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
