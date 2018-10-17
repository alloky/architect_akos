#ifdef MEM_PROTECTION

#include "logging.h"
#define MEMPT_CNRY_SIZE 20
#define MEMPRT_ASSERT_OK if(!isOk()) { LEV_LOG(LL_ERROR, "MEMEPROTECTION BROKEN!"); body.__strace(); }
#define MEMPRT_POISON_VALUE 255

#endif

template <class T>
class MemProtector {
#ifdef MEM_PROTECTION
private:
	int __field_cnry_top[MEMPT_CNRY_SIZE];
	std::hash<T> __class_hasher;
	size_t __current_hash;
#endif
public:
	T body;
#ifdef MEM_PROTECTION
private:
	int __field_cnry_btm[MEMPT_CNRY_SIZE];
#endif

public:
	MemProtector() :
		body() {
		__init_canaries();
		__current_hash = __class_hasher(body);
	}

	T& operator() {
		MEMPRT_ASSERT_OK();
		return body;
	}

#ifdef MEM_PROTECTION


	bool isOk() {
		return __checkCanaries() && body.isOk();
	}

private:
	bool __check_cnry(bool top = true) {
		int* __field_cnry = top ? __field_cnry_top : __field_cnry_btm;
		for (size_t i = 0; i < MEMPT_CNRY_SIZE; ++i) {
			if (__field_cnry[i] != 0xBEDABEDA) {
				LEV_LOG(LL_ERROR, "STACK CANARY CHECK FAILED! "
					"INVALID WRITE TO " << (top? "TOP" : "BTM") << " CANARY "
					"IN POS : " << i);
			}
		}
	}

	bool __checkCanaries() {
		return __check_cnry() && __check_cnry(false);
	}

	void __init_canaries() {
		std::memset(__field_cnry_top, 0xBEDABEDA, ST_DBG_CNRY_SIZE);
		std::memset(__field_cnry_btm, 0xBEDABEDA, ST_DBG_CNRY_SIZE);
	}

#endif


};