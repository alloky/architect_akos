#include <iostream>

#include "logging.cpp"

int main(){
	LEV_LOG(LL_INFO, "Hello world");
	std::cout << "1\n";
	return 0; 
}
