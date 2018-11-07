#pragma once

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
			labels_addrs[__parse_label_name(line, 0)] = addr;
		}
		addr++;
	}
}

std::string Compiler::__compilation_loop(FileLineProcesser::LinesView* lv) {
	addr = 0, 
	line_num = 0;
	for (auto line : lv->lines) {
		__process_line(line);
		line_num++;
	}
	return result;
}


void Compiler::__process_line(std::string_view& line) {
	if (line.size() == 0) return;
	if (line[0] == '#') return;
	if (line[0] == ':') return;
	size_t first_word_end = line.find(" ");
	

	std::vector<std::pair<Compiler::ARG_TYPES, size_t>> args = __parse_args(line);
	size_t _tmp_reg_val;
	long long _tmp_long_val;
	
	// Codegeneration for cmd parsing
	if (0) {}
#define DEF_CMD(CMD, WORD, COND, PARSE_CODE) else if((strncmp(line.data(), WORD , first_word_end) == 0) && COND){\
		if(true) PARSE_CODE \
		__push_args(line, args); \
	}

#define PUSH_NUM(CMD) result.push_back(CPUE_CMD_NUM:: ## CMD);\
		addr++;
#define ARGS_SIZE args.size() 
#define IS_LONG(N) (args[N].first == Compiler::ARG_TYPES::LONG) 
#define IS_REG(N)  (args[N].first == Compiler::ARG_TYPES::REG)
#define IS_REG_STAR(N)  (args[N].first == Compiler::ARG_TYPES::REG_STAR)


#define PARSE_SIZE_T \
	_tmp_reg_val = atoll(line.data() + first_word_end); \
	result.append(std::string(reinterpret_cast<char*>(&_tmp_reg_val), sizeof(size_t))); \
	addr += sizeof(size_t);


#include "cpuemu-cmd-defs.h"

#undef DEF_CMD
#undef PUSH_NUM
#undef ARGS_SIZE
#undef IN_LONG
#undef IS_REG
#undef PARSE_REG
#undef PARSE_LONG
#undef PARSE_SIZE_T

}


std::vector<std::pair<Compiler::ARG_TYPES, size_t>> Compiler::__parse_args(std::string_view& line) const {
	std::vector<std::pair<Compiler::ARG_TYPES, size_t>> ret;
	bool flag = false;
	for (size_t i = 0; i < line.size(); ++i) {
		if (flag) {
			if (line[i] != ' ' && line[i] != '\n') {
				if (line[i] >= '0' && line[i] <= '9') {
					ret.push_back({ Compiler::ARG_TYPES::LONG, i });
				}
				else if (line[i] == 'r') {
					ret.push_back({ Compiler::ARG_TYPES::REG, i });
				}
				else if (line[i] == ':') {
					ret.push_back({ Compiler::ARG_TYPES::LABEL, i });
				}
				else if (line[i] == '[') {
					++i;
					if (line[i] >= '0' && line[i] <= '9') {
						ret.push_back({ Compiler::ARG_TYPES::LONG_STAR, i });
					}
					else if (line[i] == 'r') {
						ret.push_back({ Compiler::ARG_TYPES::REG_STAR, i });
					}
				}
				flag = false;
			}
		}
		else if (line[i] == ' ') {
			flag = true;
		}
	}
	return ret;
}

void Compiler::__push_args(string_view& line, std::vector<std::pair<Compiler::ARG_TYPES, size_t>>& args) {
	if (args.size() == 0) {
		return;
	}

	for (size_t i = 0; i < args.size(); ++i) {
		switch (args[i].first)
		{
		case Compiler::ARG_TYPES::LABEL:
			__push_LABEL(line, args[i].second);
			break;

		case Compiler::ARG_TYPES::LONG:
			__push_LONG(line, args[i].second);
			break;

		case Compiler::ARG_TYPES::LONG_STAR:
			__push_LONG_STAR(line, args[i].second);
			break;

		case Compiler::ARG_TYPES::REG:
			__push_REG(line, args[i].second);
			break;

		case Compiler::ARG_TYPES::REG_STAR:
			__push_REG_STAR(line, args[i].second);
			break;
		default:
			break;
		}
	}
}

string_view Compiler::__parse_label_name(string_view& line, size_t start) {
	size_t label_name_start = start + 1;
	size_t label_name_end = line.find(' ', label_name_start);
	return line.substr(label_name_start, label_name_end);
}

char Compiler::__parse_reg_name(string_view & line, size_t start)
{
	size_t value_part_start = start + 1;
	if (('a' <= line[value_part_start]) && ('d' >= line[value_part_start])) {
		return line[value_part_start] - 'a';
	}
	else {
		return atoi(line.data() + value_part_start);
	}
}

void Compiler::__push_LABEL(std::string_view& line, size_t start) {
	string_view label_name = __parse_label_name(line, start);
	if (labels_addrs.find(label_name) == labels_addrs.end()) {
		throw std::exception("Invalid label name");
	}
	size_t lbl_addr = labels_addrs[label_name];
	push_bytes<size_t>(lbl_addr);
}

void Compiler::__push_LONG(std::string_view & line, size_t start)
{
	long long _tmp_long_val = atoll(line.data() + start); 
	push_bytes<long long>(_tmp_long_val);
}

void Compiler::__push_LONG_STAR(std::string_view & line, size_t start)
{
	throw std::exception("not implemented");
}

void Compiler::__push_REG(std::string_view & line, size_t start)
{
	char _tmp_reg_val = __parse_reg_name(line, start); 
	push_bytes<char>(_tmp_reg_val);
}

void Compiler::__push_REG_STAR(std::string_view & line, size_t start)
{
	char _tmp_reg_val = __parse_reg_name(line, start);
	push_bytes<char>(_tmp_reg_val);
}

template <class T>
void Compiler::push_bytes(T v) {
	result.append(std::string(reinterpret_cast<char*>(&v), sizeof(T)));
	addr += sizeof(T);
}