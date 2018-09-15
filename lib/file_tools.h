#include <stdio.h>

#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <errno.h>
#include <types.h>
#include <string.h>

const int FR_NOT_FOUND = -1;
const int FR_READ_ERROR = -2;
const int FR_EMPY_FILE = -3;

int get_file_size(const char* filename);

int get_string_count(const char* buff);

int file_to_ptr_arr(const char* filename, char*** string_arr, int* total_size);
