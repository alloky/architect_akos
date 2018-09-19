#include <assert.h>

#include "file_tools.h"
#include "logging.h"

#ifdef WIN
    #include <io.h>
    #define fr_stat _stat
    #define fr_open _open
    #define fr_read _read
    #define fr_write _write
    #define fr_close _close
    #define FR_RDONLY _O_RDONLY 
    #define FR_WRTCREAT (_O_WRONLY | O_CREAT)
    #define FR_RULES 0777//(_S_IREAD | _S_IWRITE)
#else
    #include <unistd.h>
    #define fr_stat stat
    #define fr_open open
    #define fr_read read
    #define fr_write write
    #define fr_close close 
    #define FR_RDONLY O_RDONLY
    #define FR_RULES 0777
    #define FR_WRTCREAT (O_WRONLY | O_CREAT)
#endif
    

void _mark_ptrs(char* buffer, char** string_arr);
int _process_error(const char* filename, int code);

int get_file_size(const char* filename){
    
    struct fr_stat file_info;
    int status = fr_stat(filename, &file_info);
    
    int code = errno;

    if(status == 0){
        return file_info.st_size;
    }
    
    return _process_error(filename, code);
}

size_t get_lines_count(const char* buff){
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

int read_file(const char* filename, char** buffer, size_t* total_size){
    int file_size = get_file_size(filename);
    
    if(file_size < 0){
        return file_size;
    }

    if(file_size == 0){
        LEV_LOG(LL_ERR, "File \"" << filename << "\" is empy");
        return FR_EMPTY_FILE;
    }


    int fd = 0;
    
    fd = fr_open(filename, FR_RDONLY);
    
    if( fd == -1 ){
        int code = errno;
        return _process_error(filename, code);
    }
    
    assert(buffer != NULL);
    *buffer = new char[file_size + 1];
    assert(*buffer != NULL);    
    *total_size = file_size;    

    int read_size = 0;
    read_size = fr_read(fd, *buffer, file_size);
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
    
    *(*buffer + (read_size)) = '\0';
    return 0;
}

int write_to_file(const char* filename, char* buffer, unsigned int size){
    
    int fd = 0;
    
    fd = fr_open(filename, FR_WRTCREAT);
    
    if( fd == -1 ){
        int code = errno;
        return _process_error(filename, code);
    }
    
    assert(buffer != NULL);    

    for(char* cur = buffer; cur < buffer + size; ++cur){
        if(*cur == '\0'){
            *cur = '\n';
        }
    }
    
    int write_size = 0;
    write_size = fr_write(fd, buffer, size);
    fr_close(fd);
    
    int ret_code = 0;

    if(write_size == -1){
        int code = errno;
        ret_code = _process_error(filename, code);
    }

    for(char* cur = buffer; cur < buffer + size; ++cur){
        if(*cur == '\n'){
            *cur = '\0';
        }
    }

    return ret_code;
}

int multi_write(const char* filename, char** ptr_arr, size_t size){
    
    assert(ptr_arr != NULL);

    int fd = 0;
    fd = fr_open(filename, FR_WRTCREAT, FR_RULES);
    if( fd == -1 ){
        int code = errno;
        return _process_error(filename, code);
    }

    for(size_t i = 0; i < size; ++i){
        assert(ptr_arr[i] != NULL);
        int len = strlen(ptr_arr[i]);
        ptr_arr[i][len] = '\n';
        if((fr_write(fd, ptr_arr[i], len + 1)) == -1){
            int code = errno;
            return _process_error(filename, code);
        }
        ptr_arr[i][len] = '\0';
    }

    if(fr_close(fd) != 0){
        int code = errno;
        return _process_error(filename, code);   
    }

    return 0;
}

size_t make_ptr_arr( char* buffer, char*** ptr_arr){
    size_t s_count = get_lines_count(buffer);
    *ptr_arr = new char*[s_count];
    assert(ptr_arr != NULL); 
    _mark_ptrs(buffer, *ptr_arr); 
    return s_count;
}

void _mark_ptrs(char* buffer, char** string_arr){
    char* cur = buffer;
    size_t i = 1;
    string_arr[0] = buffer;
    while(*cur != '\0'){
        if(*cur == '\n'){
            string_arr[i] = cur + 1;
            *cur = '\0';
            ++i;
        }
        ++cur;
    }
}

int _process_error(const char* filename, int code){
  	LEV_LOG(LL_ERR, "While reading file \"" << filename << "\" error occured");
    LEV_LOG(LL_ERR, strerror(code));

    if(code == ENOENT){
        return FR_NOT_FOUND;
    }

    return FR_READ_ERR;
}
