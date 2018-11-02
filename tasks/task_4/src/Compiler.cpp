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
	
	// Codegeneration for cmd parsing
	if (0) {}
#define DEF_CMD(CMD, WORD, PARSE_CODE, PROC_CODE) else if(strncmp(line.data(), WORD , first_word_end) == 0){\
		result.push_back(CPUE_CMD_NUM:: ## CMD);\
		addr++;\
		if(true) PARSE_CODE \
	}
#define PARSE_LONG \
	long long _tmp_long_val = atoll(line.data() + first_word_end); \
	result.append(std::string(reinterpret_cast<char*>(&_tmp_long_val), sizeof(long long))); \
	addr += sizeof(long long);
#define PARSE_LABEL string_view label_name = __parse_label_name(line);\
		if (labels_addrs.find(label_name) == labels_addrs.end()) {\
			throw std::exception("Invalid label name on line : " + line_num);\
		}\
	size_t lbl_addr = labels_addrs[label_name];\
	result.append(std::string(reinterpret_cast<char*>(&lbl_addr), sizeof(size_t)));\
	addr += sizeof(size_t);
#include "cpuemu-cmd-defs.h"
#undef DEF_CMD
	
}

string_view Compiler::__parse_label_name(string_view& line) {
	size_t label_name_start = line.find(':') + 1;
	size_t label_name_end = line.find(':', label_name_start);
	return line.substr(label_name_start, label_name_end);
}