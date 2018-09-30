#include "smart_descriptor.h"
#include "logging.h"

int SmartDescriptor::open_read(const std::string& path){
	return ft_open(&file, path.c_str(), "r+");
}

int SmartDescriptor::open_write(const std::string& path){
	return ft_open(&file, path.c_str(), "w+");
}


/*int SmartDescriptor::read(string_view* s_view, size_t len){
	int r_size = ft_read(file, s_view->buff, len);
	s_view.size = r_size;
	return r_size;	
}*/

int SmartDescriptor::read_all(char** buff, size_t* size){
	int file_size = ft_size(file);
	int r_size = ft_read(file, buff, file_size);
	*size = r_size;
	return r_size;
}

int SmartDescriptor::write(string_view& s_view, char sep='\n'){
	return ft_write(file, s_view.data(), s_view.size(), sep);
}

int SmartDescriptor::close(){
	return ft_close(file);
}
