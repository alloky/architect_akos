#include <assert.h>

#ifdef STACK_DEBUG

#include "logging.h"
#define ST_DBG_CNRY_SIZE 20
#define ST_ASSERT_OK if(!__st_isOk(this)) { LEV_LOG(LL_ERROR, "STACK IS NOT OK!"); __strace(); }
#define ST_POISON_VALUE 255
bool __st_isOk(Stack* st);

#endif

template <typename T>
class Stack {
	#ifdef STACK_DEBUG
	int __field_cnry_top[ST_DBG_CNRY_SIZE];
	Stack<T>* fake_this;
	int* __buffer_cnry_top;
	int* __buffer_cnry_btm;
	std::hash<T> buf_hasher;
	#endif
	
	size_t capacity;
	size_t size;
	T* buffer;
		
	#ifdef STAKC_DEBUG
	int __field_cnry_btm[ST_DBG_CNRY_SIZE];
	#endif
public:
	explicit Stack() :
		size(0),
		buffer(nullptr),
		capacity(0) {
		#ifdef STACK_DEBUG
		fake_this = this; 
		std::memset(__field_cnry_top, 0xBEDABEDA, ST_DBG_CNRY_SIZE);		
		std::memset(__field_cnry_btm, 0xBEDABEDA, ST_DBG_CNRY_SIZE);
		#endif
	}
	
	bool pop() {
		#ifdef STACK_DEBUG
		ST_ASSERT_OK();
		#endif
		
		if (size == 0) {
			return false;
		}
		assert(buffer != nullptr);
		--size;

		#ifdef STACK_DEBUG
		__make_poison(size);
		ST_ASSERT_OK();
		#endif	

		return true;
	}	

	void push(T& element){
		#ifdef STACK_DEBUG
		ST_ASSERT_OK();
		#endif
		
		if(size == capacity){
			__expand();
		}
		assert(buffer != nullptr);
		buffer[size] = element;
		++size;		

		#ifdef STACK_DEBUG
		ST_ASSERT_OK();
		#endif
	}
	
	T top(){
		#ifdef STACK_DEBUG
		ST_ASSERT_OK();
		#endif

		assert(buffer != nullptr);
		return buffer[size-1];	
	}

private:
	void __expand(){
		#ifdef STACK_DEBUG
		__expand_in_debug();
		return;
		#endif
		
		capacity *= 2;
		T* new_buff = new T[capacity];
		if(new_buff == nullptr){
			throw std::bad_alloc("Stack expand failed: new returned nullptr");
		}
		std::memcpy(new_buffer, buffer, sizeof(T)*(capacity/2));
	}
	
	#ifdef STACK_DEBUG
	void __expand_in_debug(){
		ST_ASSERT_OK();

		capacity *= 2;
		void* new_mem = new[sizeof(T)*capacity + ST_DBG_CNRY_SIZE*2*sizeof(int*)];
		if(new_mem == nullptr){
			LEV_LOG(LL_ERROR, "STACK ERROR : NO MEM WHILE EXPANDING");
			throw std::bad_alloc("Stack expand failde: new returned nullptr");
		}

		__buffer_cnry_top = (int*) new_mem;
		std::memset(__buffer_cnry_top, 0xBEDABEDA, ST_DBG_CNRY_SIZE);
		
		T* new_buffer = (T*)((int*)new_mem + ST_DBG_CNRY_SIZE);
		std::memcpy(new_buffer, buffer, sizeof(T)*(capacity/2));
		std::fill(new_buffer + capacity, ST_POISON_VALUE, (capacity - size));		

		__buffer_cnry_btm = (int*)(new_buffer + capacity);
		std::memset(__buffer_cnry_btm, 0xBEDABEDA, ST_DBG_CNRY_SIZE);
		

		ST_ASSERT_OK();	
	}
	

	void __strace(const char* msg){
		LEV_LOG(LL_DEBUG, "Stack class strace triggered");
		LEV_LOG(LL_DEBUG, "message : " << msg);
		
		std::cout << "Stack [" << this << "]" << " (" << (__st_isOk() ? "Ok" : "Not ok") << ")  {\n";
		std::cout << "    " << "__field_cnry_top = " << __field_cnry_top[0] << (__check_top_cnry()? "(ok)" : "(spoiled)") << "\n";
		std::cout << "    " << "fake_this = " << fake_this << "\n"; 			
		std::cout << '\n';

		std::cout << "    " << "capacity  = " << capacity << "\n";
		std::cout << "    " << "size = " << size << "\n";
		std::cout << "    " << "fake_this = " << fake_this << "\n";

		std::cout << std::endl;

		std::cout << "    " << "buffer [" << buffer << "] {\n";
		for(size_t i = 0; i < capacity; ++i){
			std::cout << "    " << (i < size ? "*" : " ");
			std::cout << "[" << i << "] : " << buffer[i];
			std::cout << (buffer[i] == ST_POISON_VALUE ? " " : "(POISON)");
			std::cout << '\n';
		}
		std::cout << "    " << "}"; 
		std::cout << std::endl;
		std::cout << "    " << "__field_cnry_btm = " << __field_cnry_btmp[0] << (__check_btm_cnry()? "(ok)" : "(spoiled)") << "\n";
		std::cout << "}";

		std::cout << std::endl;

		LEVL_LOG(LL_DEBUG, "End of Stack strace");
	}

	bool __check_btm_cnry(){
		for (size_t i = 0; i < ST_DBG_CNRY_SIZE; ++i){
			if(__field_cnry_btm[i] != 0xBEDABEDA){
				LEV_LOG(LL_ERROR, "STACK CANARY CHECK FAILED!"
								  "INVALID WRITE TO BOTTOM CANARY"
								  "IN POS : " << i);
			}
		}
	}

	bool __check_top_cnry(){
		for (size_t i = 0; i < ST_DBG_CNRY_SIZE; ++i){
			if(__field_cnry_top[i] != 0xBEDABEDA){
				LEV_LOG(LL_ERROR, "STACK CANARY CHECK FAILED!"
								  "INVALID WRITE TO TOP CANARY"
								  "IN POS : " << i);
			}
		}
	} 

	bool __check_buffer_top_cnry(){
		for (size_t i = 0; i < ST_DBG_CNRY_SIZE; ++i){
			if(__buffer_cnry_top[i] != 0xBEDABEDA){
				LEV_LOG(LL_ERROR, "STACK BUFFER CANARY CHECK FAILED!"
					              "INVALID WRITE TO BUFFER TOP CANARY"
					              "IN POS : " << i);
			}
		}
	}

	bool __check_buffer_btm_cnry(){
		for (size_t i = 0; i < ST_DBG_CNRY_SIZE; ++i){
			if(__buffer_cnry_btm[i] != 0xBEDABEDA){
				LEV_LOG(LL_ERROR, "STACK BUFFER CANARY CHECK FAILED!"
					              "INVALID WRITE TO BUFFER BTM CANARY"
					              "IN POS : " << i);
				return false;
			}
		}
		return true;
	} 

	bool __check_buffer_hash(){

	}

	#endif
};

bool __st_isOk(Stack* st){
	return st != nullptr &&
	st->fake_this == st &&
	st->size >= 0 &&
	st->capacity >= 0 &&
	st->capacity >= size &&
	st->buffer != nullptr && 
	st->__check_btm_cnry() &&
	st->__check_top_cnry() &&
	st->__check_buffer_top_cnry() &&
	st->__check_buffer_btm_cnry() &&
	st->__check_buffer_hash();
}
