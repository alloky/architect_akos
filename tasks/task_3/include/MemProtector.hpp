#ifdef MEM_PROTECTION

#include "logging.h"
#define MEMPT_CNRY_SIZE 20
#define MEMPRT_ASSERT_OK if(!isOk()) { LEV_LOG(LL_ERR, "MEMEPROTECTION BROKEN!"); body.__strace("bad news everyone!"); }
#define MEMPRT_POISON_VALUE 255

#endif

template <class T>
class MemProtector {
#ifdef MEM_PROTECTION
private:
	int __field_cnry_top[MEMPT_CNRY_SIZE];
#endif
public:
	T body;
#ifdef MEM_PROTECTION
private:
	int __field_cnry_btm[MEMPT_CNRY_SIZE];
#endif

public:
	MemProtector() {
#ifdef MEM_PROTECTION
		__init_canaries();
#endif
	}

	~MemProtector() {
	}

	T& operator()() {
		MEMPRT_ASSERT_OK;
		return body;
	}

#ifdef MEM_PROTECTION
public:
	bool isOk() {
		return __check_canaries() && body.isOk();
	}

private:
	bool __check_cnry(bool top = true) {
		int* __field_cnry = top ? __field_cnry_top : __field_cnry_btm;
		for (size_t i = 0; i < MEMPT_CNRY_SIZE; ++i) {
			if (__field_cnry[i] != 0xBEDABEDA) {
				LEV_LOG(LL_ERR, "STACK CANARY CHECK FAILED! "
					"INVALID WRITE TO " << (top? "TOP" : "BTM") << " CANARY "
					"IN POS : " << i);
				return false;
			}
		}
		return true;
	}

	bool __check_canaries() {
		return __check_cnry() && __check_cnry(false);
	}

	void __init_canaries() {
		std::memset(__field_cnry_top, 0xBEDABEDA, MEMPT_CNRY_SIZE);
		std::memset(__field_cnry_btm, 0xBEDABEDA, MEMPT_CNRY_SIZE);
	}

	void __strace(const char* msg) {
		LEV_LOG(LL_DEBUG, "MemProtector class strace triggered");
		LEV_LOG(LL_DEBUG, "message : " << msg);
		
		std::cout << "    " << "__field__cnry_top = " << __cnry_top[0] << (__check_cnry(true) ? "(ok)" : "(spoiled)") << "\n";

		std::cout << "Body (" << (__st_isOk() ? "OK" : "NOT OK") << ") {" << "\n";
		body.__strace(msg);
		
		std::cout << "    " << "__field__cnry_btm = " << __cnry_btmp[0] << (__check_cnry() ? "(ok)" : "(spoiled)") << "\n";
		std::cout << "}";

		std::cout << std::endl;

		LEVL_LOG(LL_DEBUG, "End of MemProtector strace");
	}

#endif


};