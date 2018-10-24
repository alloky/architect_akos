

#include "compiler.h"
#include "cpuemu-cmds.h"

void Compiler::compile(const char* name, const char* path, const char* out_dir) {
	FileLineProcesser flp;
	flp.open(path + std::string("/") + name + ".txt");
	flp.read_lines();
	auto lv = flp.make_LinesView();

	std::string byte_code = getByteCode(lv);
	__write_to_out(byte_code, path, out_dir, name);
}


void Compiler::__write_to_out(const std::string& byte_code, const char* path, const char* out_dir, const char* name) {
	SmartDescriptor sd_writer;
	sd_writer.open_write(out_dir + std::string("/") + name + "_" + version + ".asmbin");
	sd_writer.write(byte_code, '\0');
	sd_writer.close();
}

std::string Compiler::getByteCode(FileLineProcesser::LinesView* lv) {
	__get_labels_addresses(lv);
	return __compilation_loop(lv);
}

 void Compiler::__get_labels_addresses(FileLineProcesser::LinesView* lv) {
	size_t addr = 0;
	for (auto line : lv->lines) {
		if (line.size() == 0) continue;
		if (line[0] == '#') continue;
		if (line[0] == ':') {
			labels_addrs[__parse_label_name(line)] = addr;
		}
		addr++;
	}
}

std::string Compiler::__compilation_loop(FileLineProcesser::LinesView* lv) {
	std::string result;
	size_t addr = 0, 
	   line_num = 0;
	for (auto line : lv->lines) {
		__process_line(line, result, addr, line_num);
		line_num++;
	}
	return result;
}

void Compiler::__process_line(std::string_view& line, std::string& result, size_t& addr, size_t line_num) {
	if (line.size() == 0) return;
	if (line[0] == '#') return;
	if (line[0] == ':') return;
	size_t first_word_end = line.find(" ");
	if (strncmp(line.data(), CPUE_ADD_WORD, first_word_end) == 0) {
		result.push_back(CPUE_ADD_NUM);
	}
	else if (strncmp(line.data(), CPUE_SUB_WORD, first_word_end) == 0) {
		result.push_back(CPUE_SUB_NUM);
	}
	else if (strncmp(line.data(), CPUE_MUL_WORD, first_word_end) == 0) {
		result.push_back(CPUE_MUL_NUM);
	}
	else if (strncmp(line.data(), CPUE_DIV_WORD, first_word_end) == 0) {
		result.push_back(CPUE_DIV_NUM);
	}
	else if (strncmp(line.data(), CPUE_MOD_WORD, first_word_end) == 0) {
		result.push_back(CPUE_MOD_NUM);
	}
	else if (strncmp(line.data(), CPUE_MOV_WORD, first_word_end) == 0) {
		result.push_back(CPUE_MOV_NUM);
		addr++;
		result.append(std::string(reinterpret_cast<char*>((size_t)atoll(line.data() + first_word_end)), sizeof(size_t)));
	}
	else if (strncmp(line.data(), CPUE_JMP_WORD, first_word_end) == 0) {
		result.push_back(CPUE_JMP_NUM);
		addr++;
		string_view label_name = __parse_label_name(line);
		if (labels_addrs.find(label_name) == labels_addrs.end()) {
			throw std::exception("Invalid label name on line : " + line_num);
		}
		result.append(std::string(reinterpret_cast<char*>((size_t)labels_addrs[label_name]), sizeof(size_t)));
	}
	else if (strncmp(line.data(), CPUE_JL_WORD, first_word_end) == 0) {
		result.push_back(CPUE_JL_NUM);
		addr++;
		string_view label_name = __parse_label_name(line);
		if (labels_addrs.find(label_name) == labels_addrs.end()) {
			throw std::exception("Invalid label name on line : " + line_num);
		}
		result.append(std::string(reinterpret_cast<char*>((size_t)labels_addrs[label_name]), sizeof(size_t)));
	}
	else if (strncmp(line.data(), CPUE_JLE_WORD, first_word_end) == 0) {
		result.push_back(CPUE_JLE_NUM);
		addr++;
		string_view label_name = __parse_label_name(line);
		if (labels_addrs.find(label_name) == labels_addrs.end()) {
			throw std::exception("Invalid label name on line : " + line_num);
		}
		result.append(std::string(reinterpret_cast<char*>((size_t)labels_addrs[label_name]), sizeof(size_t)));
	}
	else if (strncmp(line.data(), CPUE_JEQ_WORD, first_word_end) == 0) {
		result.push_back(CPUE_JEQ_NUM);
		addr++;
		string_view label_name = __parse_label_name(line);
		if (labels_addrs.find(label_name) == labels_addrs.end()) {
			throw std::exception("Invalid label name on line : " + line_num);
		}
		result.append(std::string(reinterpret_cast<char*>((size_t)labels_addrs[label_name]), sizeof(size_t)));
	}
	else if (strncmp(line.data(), CPUE_JGE_WORD, first_word_end) == 0) {
		result.push_back(CPUE_JGE_NUM);
		addr++;
		string_view label_name = __parse_label_name(line);
		if (labels_addrs.find(label_name) == labels_addrs.end()) {
			throw std::exception("Invalid label name on line : " + line_num);
		}
		result.append(std::string(reinterpret_cast<char*>((size_t)labels_addrs[label_name]), sizeof(size_t)));
	}
	else if (strncmp(line.data(), CPUE_JG_WORD, first_word_end) == 0) {
		result.push_back(CPUE_JG_NUM);
		addr++;
		string_view label_name = __parse_label_name(line);
		if (labels_addrs.find(label_name) == labels_addrs.end()) {
			throw std::exception("Invalid label name on line : " + line_num);
		}
		result.append(std::string(reinterpret_cast<char*>((size_t)labels_addrs[label_name]), sizeof(long long)));
	}
	else if (strncmp(line.data(), CPUE_PUSH_WORD, first_word_end) == 0) {
		result.push_back(CPUE_PUSH_NUM);
		addr++;
		long long val = atoll(line.data() + first_word_end);
		result.append(std::string(reinterpret_cast<char*>(&val), sizeof(long long)));
	}
	else if (strncmp(line.data(), CPUE_POP_WORD, first_word_end) == 0) {
		result.push_back(CPUE_POP_NUM);
	}
	addr++;
}

string_view Compiler::__parse_label_name(string_view& line) {
	size_t label_name_start = line.find(':') + 1;
	size_t label_name_end = line.find(':', label_name_start);
	return line.substr(label_name_start, label_name_end);
}