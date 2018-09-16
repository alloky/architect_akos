#include <assert.h>

#include "filereader.h"
#include "logging.g"

#ifdef WIN
    #define fr_stat _stat
    #define fr_open _open
    #define fr_read _read
    #define fr_close _close 
#else
    #define fr_stat stat
    #define fr_open open
    #define fr_read read
    #define fr_close close 
#endif
    

void _mark_ptrs(char** string_arr, char* buffer);
int _process_error(const char* filename, int code);

int get_file_size(const char* filename){
    
    struct _stat file_info;
    status = fr_stat(filename, file_info);
    
    int code = errno;

    if(status == 0){
        return file_info.st_size;
    }
    
    return _process_error(filename, code);
}

size_t get_line_count(const char* buff){
    size_t ind = 0;
    size_t count = 0;    
    
    assert(buff != NULL);

    while(buff[ind] != '\0'){
        if(buff[ind] == '\n'){
            ++count;
        }
        ++ind;
    }
    
    return count + 1; 
}

int read_file(const char* filename, char** buffer, int* total_size){
    int file_size = get_file_size(filename);
    
    if(file_size < 0){
        return file_size;
    }

    if(file_size == 0){
        LEV_LOG(LL_ERR, "File \"" << filename << "\" is empy");
        return FR_EMPTY_FILE;
    }


    int fd = 0;
    
    fd = fr_open(filename, _O_RDONLY);
    
    if( fd == -1 ){
        int code = errno;
        return _process_error(filename, code);
    }
    
    buffer = new char[file_size + 1];
    assert(buffer != NULL);    
    *total_size = file_size;    

    int read_size = 0;
    read_size = fr_read(fd, buffer, file_size);
    fr_close(fd);

    if(read_size == 0){
        LEV_LOG(LL_ERR, "File \"" << filename << "\" is empy");
        delete buffer;
        return FR_EMPTY_FILE;
    } else if(read_size == -1){
        int code = errno;
        delete buffer;
        return _process_error(filename, code);
    }
    
    buff[read_size - 1] = '\0';

    return 0;
}

size_t make_ptr_arr(const char* buffer, char** ptr_arr){
    size_t s_count = get_line_count(buffer);
    assert(string_arr != NULL); 
    ptr_arr = new char*[s_count];
    _mark_ptrs(buffer, ptr_arr); 
    return s_count;
}

void _mark_ptrs(char* buffer, char** string_arr){
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
