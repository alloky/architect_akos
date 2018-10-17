#include <assert.h>
#include "ProtectedCArray.hpp"

#define MEM_PROTECTION 1
#ifdef MEM_PROTECTION

#include "logging.h"
#define ST_ASSERT_OK if(!isOk()) { LEV_LOG(LL_ERR, "STACK IS NOT OK!"); __strace("bad news"); return false; }
#define ST_POISON_VALUE 255

#endif



template <typename T>
class Stack {
#ifdef MEM_PROTECTION
	Stack<T>* __fake_this{ nullptr };
	size_t __hash_sum{ 0 };
	std::hash<T> __buff_hasher{};
#endif

	ProtectedCArray<T> buffer{};


public:
	Stack() :
		__fake_this(this)
	{
	}

	~Stack() 
	{
	}
	
	bool pop() {
		#ifdef MEM_PROTECTION
		ST_ASSERT_OK;
		#endif
		
		if (buffer.size == 0) {
			return false;
		}
		--buffer.size;

		#ifdef MEM_PROTECTION
		buffer.__make_poison(buffer.size);
		__hash_sum = __calc_hash();
		ST_ASSERT_OK;
		#endif	

		return true;
	}	

	bool push(const T& element){
		#ifdef MEM_PROTECTION
		ST_ASSERT_OK;
		#endif
		
		if(buffer.size == buffer.capacity){
			buffer.expand();
		}
		buffer[buffer.size] = element;
		++buffer.size;		

		#ifdef MEM_PROTECTION
		__hash_sum = __calc_hash();
		ST_ASSERT_OK;
		#endif

		return true;
	}
	
	bool top(T& el){
		#ifdef MEM_PROTECTION
		ST_ASSERT_OK;
		#endif

		assert(buffer.size != 0);
		el = buffer[buffer.size-1];	
		return true;
	}

private:
	void __expand(){
		buffer.expand();
	}
	
	#ifdef MEM_PROTECTION
public:
	void __strace(const char* msg) {
		LEV_LOG(LL_DEBUG, "Stack class strace triggered");
		LEV_LOG(LL_DEBUG, "message : " << msg);

		std::cout << "Stack [" << this << "]" << " (" << (isOk() ? "Ok" : "Not ok") << ")  {\n";
		std::cout << "    " << "fake_this = " << __fake_this << "\n";
		std::cout << '\n';

		std::cout << "    " << "capacity  = " << buffer.capacity << "\n";
		std::cout << "    " << "size = " << buffer.size << "\n";
		
		std::cout << std::endl;

		std::cout << "    " << "buffer [" << buffer.buffer << "] {\n";
		for (size_t i = 0; i < buffer.capacity; ++i) {
			std::cout << "    " << (i < buffer.size ? "*" : " ");
			std::cout << "[" << i << "] : " << buffer[i];
			std::cout << (std::memcmp(&buffer[i], &PRTARRAY_POISON_VALUE, sizeof(T)) ? " " : "(POISON)");
			std::cout << '\n';
		}
		std::cout << "    " << "}";
		std::cout << std::endl;
		std::cout << "}";

		std::cout << std::endl;

		LEV_LOG(LL_DEBUG, "End of Stack strace");
	}
private:
	size_t __calc_hash(){
		size_t hash_sum = 0;
		
		hash_sum += 3 * hash_sum + (size_t)buffer.buffer;
		hash_sum += 3 * hash_sum + buffer.capacity;
		hash_sum += 3 * hash_sum + buffer.size;
		hash_sum += 3 * hash_sum + (size_t)__fake_this;

		if (buffer.capacity < 0) {
			return hash_sum;
		}

		assert(buffer.buffer != nullptr);

		for (size_t i = 0; i < buffer.capacity; ++i) {
			hash_sum += 3 * hash_sum + __buff_hasher(buffer[i]);
		}

		return hash_sum;
	}

	bool __check_hash() {
		return __hash_sum == __calc_hash();
	}

public:
	bool isOk() {
		return this &&
			__fake_this == this &&
			buffer.size >= 0 &&
			buffer.capacity >= 0 &&
			buffer.capacity >= buffer.size &&
			buffer.isOk();
			__check_hash();
	}

	#endif
};


#ifdef MEM_PROTECTION


#endif // STACK_DEBUG

