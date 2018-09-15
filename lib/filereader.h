#include "file_tools.h"

class FileReader {
	char* buffer;
	unsigned int total_size;	
public:
	class LineView {
		char** ptrs;
		int size;
	public:
		size_t size() const;
		char* operator[](int i);
	};

	LineView&& make_LineView();
	FileReader(const char* filename);
	~FileReader();
};
