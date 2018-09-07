#include <iostream>

#include "logging.h"
#include "polynom.h"

int main(){
	LEV_LOG(LL_INFO, "Tool for polynom solving");
	
	int deg = 0;

	Polynom poly;
	LEV_LOG(LL_INFO, "Input poly deg:");
	std::cin >> poly.deg;
	
	LEV_LOG(LL_INFO, "Input coefs (float point, from constant to maximum degree):");

	std::string s_coefs;
	std::getline(std::cin, s_coefs);
	poly.parse_coefs(s_coefs);
	
	int n_roots = poly.find_roots();

	switch()

	return 0; 
}
