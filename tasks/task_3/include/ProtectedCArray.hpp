#ifdef MEM_PROTECTION

#include "logging.h"
#define PRTARRAY_CNRY_SIZE 20
#define PRTARRAY_POISON_VALUE 255

#endif


template <class T>
class ProtectedCArray {
#ifdef MEM_PROTECTION
	char* raw_data;
	int* __cnry_top;
	T* buffer;
	int* __cnry_btm;

	void expand(size_t size) {
		char* new_mem = new char[sizeof(T)*size + PRTARRAY_CNRY_SIZE * 2 * sizeof(int)]();
		if (new_mem == nullptr) {
			LEV_LOG(LL_ERROR, "STACK ERROR : NO MEM WHILE EXPANDING");
			throw std::bad_alloc("Stack expand failde: new returned nullptr");
		}

		__cnry_top = (int*)new_mem;
		std::memset(__cnry_top, 0xBEDABEDA, PRTARRAY_CNRY_SIZE);

		T* new_buffer = (T*)((int*)new_mem + PRTARRAY_CNRY_SIZE);
		std::copy(new_buffer, new_buffer + size, buffer);
		std::fill(new_buffer + size, new_buffer + 2*size, PRTARRAY_POISON_VALUE);
		delete buffer;

		__cnry_btm = (int*)(new_buffer + 2*size);
		std::memset(__cnry_btm, 0xBEDABEDA, PRTARRAY_CNRY_SIZE);
	}
#else

#endif
};