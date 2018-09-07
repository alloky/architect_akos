#include "mathtools.h"

/** @brief see @link mathtool.h @endlink for docs
*/
int solve_linear(double a, double b, double* root_1){
	if (a == 0){
		if(b == 0)
			return MT_INF_ROOTS;
		return 0;
	}
	*root = -b / a;
	return 1; 
}


/** @brief see @link mathtool.h @endlink for docs
*/
int solve_square(double a, double b, double c, double* root_1, double* root_2){
	
	//TODO: verify for NaNs

	if(a == 0.0){
		return solve_linear(b, c, root_1);
	}

	double D = b*b - 4*a*c;

	if (D < 0){
		return 0;
	}
	
	if (D > 0){
		root_1 = -b/(2.0*a) + sqrt(D);
		root_2 = root_1 - 2.0*sqrt(D);
		return 2;
	}

	root_1 = -b/(2.0*a);
	root_2 = root_1;
	return 1;
}