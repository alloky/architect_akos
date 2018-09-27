#include "smart_descriptor.h"

int SmartDescriptor::open(const std::string& path){
	return fr_open(&file, path, "r+");
}

int SmartDescriptor::read(string_view* s_view, size_t len){
	int r_size = fr_read(file, s_view.buff, len);
	s_view.size = r_size;
	return r_size;	
}

int SmartDescriptor::read_full(string_view* s_view){
	int file_size = ft_size(file);
	int r_size = fr_read(file, s_view.buff, file_size);
	s_view.size = r_size;
	return r_size;
}

int SmartDescriptor::write(string_view& s_view, char sep = '\n'){
	return fr_write(file, s_view.buff, s_view.size, sep);
}

int SmartDescriptor::close(){
	return fr_close(file);
}