#include <algorithm>
#include <utility>

#ifdef MEM_PROTECTION

#include "logging.h"
#define PRTARRAY_CNRY_SIZE 20
std::string PRTARRAY_POISON = std::string("pzdc");
#endif


template <class T>
class ProtectedCArray {
#ifdef MEM_PROTECTION
private:
	int** __cnry_top;
public:
	size_t capacity;
	size_t size;
	T* buffer{nullptr};
private:
	int** __cnry_btm;
public:
	void expand() {
		if (capacity == 0) capacity = 1;
		capacity *= 2;
		int* new_mem =(int*) (new char[sizeof(T)*capacity + PRTARRAY_CNRY_SIZE * 2 * sizeof(int*)]());
		if (new_mem == nullptr) {
			LEV_LOG(LL_ERR, "ProtectedCArray ERROR : NO MEM WHILE EXPANDING");
			throw std::exception("ProtectedCArray expand failed: new returned nullptr");
		}

		T* new_buffer = (T*)((int**)new_mem + PRTARRAY_CNRY_SIZE);
		std::move(buffer, buffer + size, new_buffer);

		for (size_t i = size; i < capacity; ++i) {
			std::memcpy(new_buffer + i, &PRTARRAY_POISON, sizeof(T));
		}

		delete[] __cnry_top;
		buffer = new_buffer;

		__cnry_top = (int**)new_mem;
		for (size_t i = 0; i < PRTARRAY_CNRY_SIZE; ++i) {
			__cnry_top[i] = (int*)MEMPRT_POISON;
		}
		
		__cnry_btm = (int**)(new_buffer + capacity);
		for (size_t i = 0; i < PRTARRAY_CNRY_SIZE; ++i) {
			__cnry_btm[i] = (int*)MEMPRT_POISON;
		}
	}

	bool __check_cnry(bool top = true) {
		if (capacity == 0) return true;
		int** __cnry = top ? __cnry_top : __cnry_btm;
		for (size_t i = 0; i < MEMPT_CNRY_SIZE; ++i) {
			if (__cnry[i] != (int*)MEMPRT_POISON) {
				LEV_LOG(LL_ERR, "ProtectedCArray CANARY CHECK FAILED! "
					"INVALID WRITE TO " << (top ? "TOP" : "BTM") << " CANARY "
					"IN POS : " << i);
				return false;
			}
		}
		return true;
	}

	void __make_poison(size_t i) {
		buffer[i] = PRTARRAY_POISON;
	}

	bool __check_canaries() {
		return __check_cnry() && __check_cnry(false);
	}

	bool isOk() {
		return __check_canaries();
	}

#else
public:
	size_t capacity{ 0 };
	size_t size{ 0 };
	T* buffer{nullptr};

	void expand() {
		if (capacity == 0) capacity = 1;
		capacity *= 2;
		T* new_buffer = new T[capacity]();
		std::move(buffer, buffer + size, new_buffer);
		delete[] buffer;
		buffer = new_buffer;
	}
#endif
	T& operator[](size_t i) {
		return buffer[i];
	}

	~ProtectedCArray() {
		if (buffer != nullptr) {
#ifdef MEM_PROTECTION
			for (size_t i = 0; i < size; ++i) {
				(buffer+i)->~T();
			}
			delete __cnry_top;
			return;
#endif
			delete[] buffer;
		}
	}
};