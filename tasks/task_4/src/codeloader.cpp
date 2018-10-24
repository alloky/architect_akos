#include "CodeLoader.h"

char* CodeLoader::load_binary(const char* path) {
	SmartDescriptor sd;
	sd.open_read(path);
	if (buff != nullptr) delete buff;
	sd.read_all(&buff, &size);
	sd.close();
	return buff;
}

CodeLoader::~CodeLoader() {
	if(buff != nullptr) delete buff;
}