#include <assert.h>

#include "filereader.h"
#include "logging.g"

void _mark_ptrs(char** string_arr, char* buffer);
int _process_error(const char* filename, int code);

int get_file_size(const char* filename){
    #ifdef WIN
        struct _stat file_info;
        status = _stat(filename, file_info);
    #else
        struct stat file_info;
        status = stat(filename, file_info);
    #endif
    
    int code = errno;

    if(status == 0){
        return file_info.st_size;
    }
    
    return _process_error(filename, code);
}

unsigned int get_string_count(const char* buff){
    size_t ind = 0;
    unsigned int count = 0;    
    
    assert(buff != NULL);

    while(buff[ind] != '\0'){
        if(buff[ind] == '\n'){
            ++count;
        }
        ++ind;
    }
    
    return count + 1; 
}

int file_to_ptr_arr(const char* filename, char*** string_arr, int* total_size){
    int file_size = get_file_size(filename);
    
    if(file_size < 0){
        return file_size;
    }

    if(file_size == 0){
        LEV_LOG(LL_ERR, "File \"" << filename << "\" is empy");
        return FR_EMPTY_FILE;
    }


    int fd = 0;
    
    #ifdef WIN
        fd = _open(filename, _O_RDONLY);
    #else
        fd = open(filename, O_RDONLY);
    #endif
    
    if( fd == -1 ){
        int code = errno;
        return _process_error(filename, code);
    }
    
    char * buffer = new char[file_size + 1];
    assert(buffer != NULL);    
    *total_size = file_size;    

    int read_size = 0;
    #ifdef WIN
        read_size = _read(fd, buffer, file_size);
    #else
        read_size = read(fd, buffer, file_size);
    #endif

    if(read_size == 0){
        LEV_LOG(LL_ERR, "File \"" << filename << "\" is empy");
        delete buffer;
        return FR_EMPTY_FILE;
    } else if(read_size == -1){
        int code = errno;
        delete buffer;
        return _process_error(filename, code;
    }
    
    unsigned int s_count = get_string_count(buffer);
    assert(string_arr != NULL); 
    *string_arr = new char*[s_count];

    _mark_ptrs(string_arr, buffer); 
    
    return s_count;
}

void _mark_ptrs(char** string_arr, char* buffer){
    char* cur = buffer;
    size_t i = 0;
    while(*cur != '\0'){
        string_arr[i] = cur;
        while(*cur != '\n'){
            ++cur;
        }
        *cur = '\0';
        ++i;
        ++cur;
    }
}

int _process_error(const char* filename, int code){
  	LEV_LOG(LL_ERR, "While reading file \"" << filename "\" error occured");
    LEV_LOG(LL_ERR, strerror(code));

    if(code == ENOENT){
        return FR_NOT_FOUND;
    }

    return FR_READ_ERR;
}
