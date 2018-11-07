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
	void __process_line(std::string_view& line);
	void __write_to_out(const std::string& byte_code, const char * path, const char * out_dir, const char * name);

public:
	enum class ARG_TYPES {
		REG,
		LONG,
		LABEL,
		REG_STAR,
		LONG_STAR
	};

private:
	std::vector<std::pair<Compiler::ARG_TYPES, size_t>> __parse_args(std::string_view& line) const;
	void __push_args(std::string_view& line, std::vector<std::pair<Compiler::ARG_TYPES, size_t>>& args);

	void __push_LABEL(std::string_view & line, size_t start);
	void __push_LONG(std::string_view & line, size_t start);
	void __push_LONG_STAR(std::string_view & line, size_t start);
	void __push_REG(std::string_view & line, size_t start);
	void __push_REG_STAR(std::string_view & line, size_t start);

	string_view __parse_label_name(string_view& line, size_t start);
	char __parse_reg_name(string_view& line, size_t start);

	size_t addr{ 0 },
	   line_num{ 0 };
	std::string result;
	template<class T>
	void push_bytes(T v);
};