#pragma once

#include "smart_descriptor.h"


class CodeLoader {
	char* buff;
public:
	size_t size;
	char* load_binary(const char* path);
	~CodeLoader();
};