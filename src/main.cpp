#include <iostream>

#include "logging.h"
#include "polynom.h"
#include "utility.h"

int main(){
	LEV_LOG(LL_INFO, "Tool for polynom solving");
	
	int deg = 0;
	
	std::cin >> deg;

	Polynom poly(deg);
	
	LEV_LOG(LL_INFO, "Input poly deg:");
	
	LEV_LOG(LL_INFO, "Input coefs (float point, from constant to maximum degree):");

	std::string s_coefs;
	std::getline(std::cin, s_coefs);
	poly.parse_coefs(s_coefs);
	
	int n_roots = poly.find_roots();

	switch(n_roots){
		case MT_INF_ROOTS:
			LEV_LOG("INFO", "Infinite number of roots");
			std::cout << -1 << "\n";
			break;
		default:
			LOG("there are " << n_roots << " roots");
			utility::cout_array<double>(poly.roots);
			break;
	}

	return 0; 
}
