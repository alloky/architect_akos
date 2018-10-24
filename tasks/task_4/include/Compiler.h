#pragma once
#include <map>

#include "smart_descriptor.h"
#include "filelineprocesser.h"

class Compiler {
	const std::string version = "0.0.1";
public:
	void compile(const char* name, const char* path, const char* out_dir);
	std::string getByteCode(FileLineProcesser::LinesView* lv);
private:
	std::map<std::string_view, size_t> labels_addrs;
	void __get_labels_addresses(FileLineProcesser::LinesView* lv);
	std::string __compilation_loop(FileLineProcesser::LinesView* lv);
	void __process_line(std::string_view& line, std::string& result, size_t& addr, size_t line_num);
	string_view __parse_label_name(string_view& line);
	void __write_to_out(const std::string& byte_code, const char * path, const char * out_dir, const char * name);
};