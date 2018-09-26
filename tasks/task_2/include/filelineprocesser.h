#include "file_tools.h"
#include <vector>
#include <algorithm>

/**
 * @brief      FileLineProcesser
 */
class FileLineProcesser {

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

		/**
		 * @brief      Write lines to file
		 */
		int write(const char* filename);
		
		LinesView* generate_bread(size_t p_count);

		LinesView() = default; //!< default constructor
		
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
	 * @return     status code, 0 if OK
	 * 
	 * Read file to internal buffer
	 */
	int read(const char* filename);

	/**
	 * @brief      Writes to file, creates it if not_exists 
	 *
	 * @param[in]  filename  path to file
	 *
	 * @return     status code, 0 if OK
	 */
	int write(const char* filename);

	FileLineProcesser();
	~FileLineProcesser();
};
