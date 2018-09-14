#include <iostream>

#include "logging.h"
#include "polynom.h"
#include "utility.hpp"

int main(){
	LEV_LOG(LL_INFO, "Tool for qaud. eq. solving");
	LEV_LOG(LL_INFO, "Solving eq.  c + bx + ax^2 = 0");
	

	int deg = 0;
	
	LEV_LOG(LL_INFO, "Input poly deg:");
	
	std::cin >> deg;
	std::cin.ignore(256, '\n');
	
	Polynom poly(deg);
	
	
	LEV_LOG(LL_INFO, "Input coefs : c b a ");

	std::string s_coefs;
	std::getline(std::cin, s_coefs);
	//std::cout << s_coefs.size() << std::endl;
	poly.parse_coefs(s_coefs);
	
	int n_roots = poly.find_roots();

	switch(n_roots){
		case MT_INF_ROOTS:
			LEV_LOG("INFO", "Infinite number of roots");
			std::cout << -1 << "\n";
			break;
		default:
			LOG("there are " << n_roots << " roots");
			utility::cout_array<double>("Roots", n_roots, poly.roots);
			break;
	}

	return 0; 
}
