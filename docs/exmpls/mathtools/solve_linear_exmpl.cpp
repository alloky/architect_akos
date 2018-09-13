/** @example solve_linear_exmpl.cpp
 * solve_linear examples
 * Examples */

#include "mathtools.h"
#include "logging.h"

int main(){
	/*
		Output:

		>> 2.0
	*/
	double a = 1.0,
		   b = -2.0,
		   root = NAN;
	int n_roots = solve_linear(a, b, &root);
	LOG(root);
}