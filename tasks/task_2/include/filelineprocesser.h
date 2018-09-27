#include <vector>
#include <algorithm>

#include "file_tools.h"
#include "my_string_view.h"

using string_view = my_string_view;

/**
 * @brief      FileLineProcesser
 */
class FileLineProcesser {

	SmartDescriptor sfd; //!< fd for io ops
	char* buffer; //!< buffer for whole file
	size_t total_size; //!< size of file in bytes

public:
	
	/**
	 * @brief      Array of lines, mutable
	 */
	class LinesView {
	public:
		std::vector<string_view> lines; //!< array of pointers to lines
		

		/**
		 * @brief      Print top <b> size </b> non-empty lines
		 *
		 * @param[in]  size  The size
		 */
		void print_top_non_empty(size_t size);
		
		LinesView() = default; //!< default constructor
		
		~LinesView() = default; //!< default destructor

		/**
		 * @brief      sort ptrs as strings
		 */
		void sort();

		/**
		 * @brief      Sort using reverse order
		 */
		void sort_backwise();
		
		LinesView* generate_bread(size_t p_count);
	
	private:
		static bool _is_prep(char c);
	};

	
	/**
	 * @brief Return new LinesView
	 */
	LinesView* make_LinesView();
	

	/**
	 * @brief      Opens file for io 
	 *
	 * @param[in]  filename  The filename
	 *
	 * @return     status code, 0 if OK
	 */
	int open(const char* filename);

	/**
	 * @brief      Read file
	 *
	 * @param[in]  filename  Path to file
	 * 
	 * @return     status code, 0 if OK
	 * 
	 * Read file to internal buffer
	 */
	int read();

	/**
	 * @brief      Writes to file, creates it if not_exists 
	 *
	 * @param[in]  filename  path to file
	 *
	 * @return     status code, 0 if OK
	 */
	int write(const LinesView& lv, char sep = '\n');

	FileLineProcesser();
	~FileLineProcesser();
};
