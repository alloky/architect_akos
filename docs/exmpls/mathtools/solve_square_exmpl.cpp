/** @example solve_square_exmpl.cpp
 * solve_linear examples
 * Examples */

#include "mathtools.h"
#include "logging.h"

int main(){
	/*
		Output:

		>> 2
		>> 1.0
		>> -1.0
	*/
	double a = 1.0,
		   b = 0.0,
		   c = -1.0
		   root_1 = 0.0,
		   root_2 = 0.0;
	int n_roots = solve_square(a, b, c, &root_1, &root_2);
	LOG(n_roots);
	LOG(root_1);
	LOG(root_2);
}