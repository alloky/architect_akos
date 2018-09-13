#include "mathtools.h"
#include "logging.h"

#include <assert.h>

/** @brief see @link mathtool.h @endlink for docs
*/
int solve_linear(double a, double b, double* root){
	
	// Check for NaNs
	assert( a == a );
	assert( b == b );
	
	if (std::abs(a) <= MT_EPS){
		if(std::abs(b) <= MT_EPS)
			return MT_INF_ROOTS;
		return 0;
	}
	*root = -b / a;
	return 1; 
}


/** @brief see @link mathtool.h @endlink for docs
*/
int solve_square(double a, double b, double c, double* root_1, double* root_2){
	
	// Check for NaNs
	assert( a == a );
	assert( b == b );
	assert( c == c );

	if(std::abs(a) <= MT_EPS){
		int n_roots = solve_linear(b, c, root_1);
		*root_2 = *root_1;
		return n_roots;
	}

	double D = b*b - 4.0*a*c;

	if (D < 0.0){
		return 0;
	}

	a *= 2.0;
	double root_base = -b/a;
	
	if (D > 0.0){
		double sqrt_D_fraq = sqrt(D)/a;
		*root_1 = root_base + sqrt_D_fraq;
		*root_2 = root_base - sqrt_D_fraq;
		return 2;
	}

	*root_1 = root_base;
	*root_2 = *root_1;
	return 1;
}
