#include "file_tools.h"
#include <string_view>

using std::string_view;

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
	 * @brief      Read 
	 *
	 * @param      buff  The buffer
	 *
	 * @return     status code, 0 if OK
	 */
	int read(string_view* buff, size_t len);
	
	/**
	 * @brief      Writes sting to open file
	 *
	 * @param      buff  The buffer
	 *
	 * @return     status code, 0 if OK
	 */
	int write(string_view& buff);
	
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