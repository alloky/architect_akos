#include "file_tools.h"
#include "my_string_view.h"

using string_view = my_string_view;

/**
 * @brief      Class for smart descriptor.
 */
class SmartDescriptor {
	FILE* file;
public:
	
	/**
	 * @brief      Opens file for WO ops
	 *
	 * @param[in]  path  path
	 *
	 * @return     status code, 0 if OK
	 */
	int open(const string& path);
	
	/**
	 * @brief      Reads from opened file
	 *
	 * @param      s_view  The s_viewer
	 * @param[in]  len   The length
	 *
	 * @return     status code, 0 if OK
	 */
	int read(string_view* s_view, size_t len);
	
	/**
	 * @brief      Reads all file
	 *
	 * @param      s_view  The s_viewer
	 *
	 * @return     status code, 0 if OK
	 */
	int read_all(string_view* s_view);

	/**
	 * @brief      Writes sting to open file
	 *
	 * @param      s_view  The s_viewer
	 *
	 * @return     status code, 0 if OK
	 */
	int write(string_view& s_view, char sep='\n');
	
	/**
	 * @brief      closes file
	 *
	 * @return     status code, 0 if OK
	 */
	int close();

	/**
	 * @brief      Returns size of file
	 *
	 * @return     size of file in bytes
	 */
	size_t get_size();
};