#include <stdio.h>

#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <errno.h>
#include <types.h>
#include <string.h>

const int FR_NOT_FOUND = -1; //!< FileNotFound error num
const int FR_READ_ERROR = -2; //!< Error while reading num
const int FR_EMPY_FILE = -3; //!< Empty file error num


/**
 * @brief      Gets the file size.
 *
 * @param[in]  filename  Path to file from running dir
 *
 * @return     The file size.
 */
int get_file_size(const char* filename);

/**
 * @brief      Count lines in string
 *
 * @param[in]  buff  String
 *
 * @return     Lines count.
 */
size_t get_lines_count(const char* buff);


/**
 * @brief      Read file to buffer
 *
 * @param[in]  filename    Path to file
 * @param      buffer	   Buffer for whole file
 * @param      total_size  Var for file size
 *
 * @return     status code, 0 if OK
 */
int read_file(const char* filename, char** buffer, int* total_size);

/**
 * @brief      Makes a pointer arr.
 *
 * @param[in]  buffer  File buffer
 * @param[in]  ptr_arr Ptr array
 *
 * @return     line count
 */
size_t make_ptr_arr(const char* buffer, char*** ptr_arr);