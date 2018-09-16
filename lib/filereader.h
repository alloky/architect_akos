#include "file_tools.h"
#include <vector>

/**
 * @brief      FileReader
 */
class FileReader {

	char* buffer; //!< body for file
	size_t total_size; //! < total size of file in bytes

public:
	
	/**
	 * @brief      Array of lines, mutable
	 */
	class LinesView {
	public:
		std::vector<char*> ptrs; //!< array of pointers to lines

	};

	
	/**
	 * @brief Return new LinesView
	 */
	LinesView&& make_LineView();
	

	/**
	 * @brief      Read file
	 *
	 * @param[in]  filename  Path to file
	 * 
	 * Read file to internal buffer
	 */
	int open(const char* filename);

	FileReader() = default;
	~FileReader();
};
