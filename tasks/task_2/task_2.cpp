
#include "logging.h"
#include "filelineprocesser.h"


FileLineProcesser::LinesView* generate_bread(FileLineProcesser::LinesView& text, size_t p_count, int seed=42);

void generate_paragraph(FileLineProcesser::LinesView& text, int* paragraph);

int find_rythm(int first_line_ind, FileLineProcesser::LinesView& text);

int choose_nearest(int first_line, int& shift, size_t size);

bool is_rythmed(FileLineProcesser::LinesView& text, int first_line_ind, int second_line_ind);

bool _is_prep(char c);



/**
 * @brief      Bredogenerator
 *
 * @return     0 if success
 * 
 * Given "data/onegin.txt" generates three files:
 * 	- alphabetly sorted file
 * 	- backwise sorted file
 * 	- bread poem in print_count paragraphs
 * 
 */
int main(){
	LOG("Programm to read files by lines");

	FileLineProcesser fr;
	int status = fr.open("data/onegin.txt");

	LEV_LOG(LL_INFO, "status : " << status);

	status = fr.read_lines();

	auto lv = fr.make_LinesView();

	LEV_LOG(LL_INFO, "Number of lines " << lv->lines.size());

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
	auto bread_lv = generate_bread(*lv, print_count);
	bread_lv->print_top_non_empty(4);
	bread_lv->write("data/onegin_bread.txt");

	return 0;
}



/**
 * @brief      Generating bread
 *
 * @param[in]  p_count  paragrahps count
 *
 * @return     Ptr to line_view
 */
FileLineProcesser::LinesView* generate_bread(FileLineProcesser::LinesView& text, size_t p_count, int seed=42){
	srand(seed);
	FileLineProcesser::LinesView* lv = new FileLineProcesser::LinesView();
	for (size_t i = 0; i < p_count; ++i){
		// generating paragraph
		int paragraph[4];
		generate_paragraph(text, paragraph);
		for(size_t j = 0; j < 4; ++j){
			lv->lines.push_back(text.lines[paragraph[j]]);
		}
		lv->lines.push_back(string_view(""));	
	}
	return lv;
}

/**
 * @brief      generate rythmed 4-line parahraph
 *
 * @param      text       The text
 * @param      paragraph  The paragraph
 */
void generate_paragraph(FileLineProcesser::LinesView& text, int* paragraph){
	for (size_t k = 0; k < 2; ++k){
		int first_line = 0;
		do {
			first_line = rand() % text.lines.size();;
		} while ( text.lines[first_line].size() == 0 );
		
		int second_line = find_rythm(first_line, text);
		paragraph[2*k] = first_line;
		paragraph[2*k+1] = second_line; 
	}
}

/**
 * @brief      Find rythm for given line
 *
 * @param[in]  first_line_ind  index of line
 * @param      text            LinesView of text
 *
 * @return     index of rythmed line
 */
int find_rythm(int first_line_ind, FileLineProcesser::LinesView& text){
	int shift = 0;
	while (true) {
		int second_line_ind = choose_nearest(first_line_ind, shift, text.lines.size());
		if(!is_rythmed(text, first_line_ind, second_line_ind)){
			continue;
		}	
		return second_line_ind;
	}
}

/**
 * @brief      Determines if rythmed.
 *
 * @param      text             The text
 * @param[in]  first_line_ind   The first line ind
 * @param[in]  second_line_ind  The second line ind
 *
 * @return     True if rythmed, False otherwise.
 */
bool is_rythmed(FileLineProcesser::LinesView& text, int first_line_ind, int second_line_ind){
	bool flag = true;
	if (text.lines[second_line_ind].size() == 0) return false;
	for (int f_i = text.lines[first_line_ind].size(),
		 s_i = text.lines[second_line_ind].size();
                 f_i >= 0 && s_i >= 0;){
		if ((text.lines[first_line_ind][s_i] == ' ') || 
                    (text.lines[second_line_ind][s_i] == ' ')){
			flag = false;
			break;
		}
		if (_is_prep(text.lines[first_line_ind][f_i])){
			--f_i;
			continue;
		}
		if (_is_prep(text.lines[second_line_ind][s_i])){
			--s_i;
			continue;
		}
		if(text.lines[first_line_ind][f_i] != text.lines[second_line_ind][s_i]){
			break;
		}
		--f_i;
		--s_i;
	}
	return flag;
}

/**
 * @brief      Choose next nearest ind in frog flip order
 *
 * @param[in]  first_line  The first line
 * @param      shift       The shift
 * @param[in]  size        The size
 *
 * @return     next index
 */
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

/**
 * @brief      Sorts LinesView alphabetly indifferent to prepositions
 *
 * @param      lv    ptr to lines view
 */
void sort(FileLineProcesser::LinesView* lv){
  std::sort(lv->lines.begin(), lv->lines.end(),
    [] (const string_view& s1, const string_view& s2){
    if(s1[0] == '\0' && s2[0] == '\0'){
    	return false;
    }
    if(s1[0] == '\0'){
    	return true;
    }
    if(s2[0] == '\0'){
    	return false;
    }
    for (size_t i = 0, j = 0; i < s1.size() && j < s2.size();) {
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


/**
 * @brief      Sorts LinesView backwise alphabetly indifferent to prepositions
 *
 * @param      lv    ptr to lines view
 */
void sort_backwise(FileLineProcesser::LinesView* lv){
  std::sort(lv->lines.begin(), lv->lines.end(),
    [] (const string_view& s1, const string_view& s2){
    int len_1 = s1.size();
    int len_2 = s2.size();
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

/**
 * @brief      Determines if char is prep.
 *
 * @param[in]  c     character
 *
 * @return     True if prep, False otherwise.
 */
bool _is_prep(char c){
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
