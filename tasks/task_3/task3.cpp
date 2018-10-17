#include "MemProtector.hpp"
#include "stack.hpp"
#include "logging.h"
#include <string>

bool test_stack();

int main() {
	LEV_LOG(LL_DEBUG, "Stack tests : " << test_stack());
	return 0;
}

bool test_stack() {
	MemProtector<Stack<std::string>> st;
	bool flag = true;
	for (size_t i = 0; i < 100; ++i) {
		flag &= st().push("hmm_" + std::to_string(i));
		if (!flag) {
			break;
		}
	}

	for (size_t i = 0; i < 100; ++i) {
		std::string el;
		flag &= st().top(el);
		if (!flag) {
			break;
		}
		std::cout << el << std::endl;
		flag &= st().pop();
		if (!flag) {
			break;
		}
	}
	LEV_LOG(LL_DEBUG, "Stack test ended");
	return flag;
}