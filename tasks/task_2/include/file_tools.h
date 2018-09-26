#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

const int FR_NOT_FOUND = -1; //!< FileNotFound error num
const int FR_READ_ERR = -2; //!< Error while reading num
const int FR_EMPTY_FILE = -3; //!< Empty file error num


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
 * @param      buffer      Buffer for whole file
 * @param      total_size  Var for file size
 *
 * @return     status code, 0 if OK
 */
int read_file(const char* filename, char** buffer, size_t* total_size);



/**
 * @brief      Writes to file, creates it if not_exists.
 *
 * @param[in]  filename  filename
 * @param      buffer    buffer
 * @param[in]  size      size
 *
 * @return     status code, 0 if ok
 * 
 * Changes '\0' to '\n' while writting and back to '\0' after
 */
int write_to_file(const char* filename, char* buffer, unsigned int size);

/**
 * @brief      Writes ptr_arr to file
 *
 * @param[in]  filename  The filename
 * @param      ptr_arr   The pointer arr
 * @param[in]  size      The size
 *
 * @return     status code
 */
int multi_write(const char* filename, char** ptr_arr, size_t size);

/**
 * @brief      Makes a pointer arr.
 *
 * @param[in]  buffer  File buffer
 * @param[in]  ptr_arr Ptr array
 *
 * @return     line count
 */
size_t make_ptr_arr(char* buffer, char*** ptr_arr);