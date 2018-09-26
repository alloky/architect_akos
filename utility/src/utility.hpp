#include <iostream>
#include <string>
#include <assert.h>

namespace utility {
	
	template <typename T>
	void cout_array(const std::string& msg, size_t size, T* arr){
		assert(arr != NULL);
		std::cout << "# " << msg << " #\n# ";
		for(size_t i = 0; i < size; ++i){
			std::cout << arr[i] << " "; 
		}
		std::cout << "\n############\n";
	}
}
