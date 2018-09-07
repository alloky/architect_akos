#include "utility.h"

template <class T>
void utility::cout_array(T* arr){
	for(T& cur : arr){
		std::cout << cur << " "
	}
	std::cout << "\n";
}
