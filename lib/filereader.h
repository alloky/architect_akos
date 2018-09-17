#include "file_tools.h"
#include <vector>
#include <algorithm>

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
		

		/**
		 * @brief      Print top <b> size </b> non-empty lines
		 *
		 * @param[in]  size  The size
		 */
		void print_top_non_empty(size_t size);

		/**
		 * @brief      sort ptrs as strings
		 */
		void sort();

		/**
		 * @brief      Sort using reverse order
		 */
		void sort_backwise();

		LinesView()  = default; //!< default constructor
		
		~LinesView() = default; //!< default destructor
	private:
		static bool _is_prep(char c);
	};

	
	/**
	 * @brief Return new LinesView
	 */
	LinesView* make_LinesView();
	

	/**
	 * @brief      Read file
	 *
	 * @param[in]  filename  Path to file
	 * 
	 * Read file to internal buffer
	 */
	int open(const char* filename);

	FileReader();
	~FileReader();
};
