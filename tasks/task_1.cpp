#include <iostream>

#include "logging.h"
#include "polynom.h"
#include "utility.hpp"

int main(){
	LEV_LOG(LL_INFO, "Tool for qaud. eq. solving");
	LEV_LOG(LL_INFO, "Solving eq. ax^2 + bx + c = 0");
	

	int deg = 2;
	
	Polynom poly(deg);
	
	
	LEV_LOG(LL_INFO, "Input coefs : a b c ");

	std::string s_coefs;
	std::getline(std::cin, s_coefs);

	poly.parse_coefs(s_coefs);
	
	int n_roots = poly.find_roots();

	switch(n_roots){
		case 0:
			LEV_LOG(LL_INFO, "Eq has no roots");
			std::cout << 0 << "\n";
			break;

		case 1:
			LEV_LOG(LL_INFO, "Eq has 1 root:");
			std::cout << poly.roots[0] << "\n";
			break;
			
		case 2:
			LEV_LOG(LL_INFO, "Eq has 2 roots:");
			utility::cout_array<double>("Roots", n_roots, poly.roots);
			break;
			
		case MT_INF_ROOTS:
			LEV_LOG(LL_INFO, "Infinite number of roots");
			std::cout << -1 << "\n";
			break;
		
		case MT_NOT_IMPL:
			LEV_LOG(LL_ERR, "Sorry, this functional is not implemented");
			std::cout << -1 << "\n";
			break;	

		default:
			LOG("there are " << n_roots << " roots");
			utility::cout_array<double>("Roots", n_roots, poly.roots);
			break;
	}

	return 0; 
}
