#include "mathtools.h"
#include "math.h"
#include "logging.h"

#include <assert.h>

/** @brief see @link mathtool.h @endlink for docs
*/
int solve_linear(double a, double b, double* root){
	
	// Check for NaNs
	assert( a == a );
	assert( b == b );
	
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
	
	LOG(a << " " << b << " " << c);
	// Check for NaNs
	assert( a == a );
	assert( b == b );
	assert( c == c );

	if(a == 0.0){
		int n_roots = solve_linear(b, c, root_1);
		*root_2 = *root_1;
		return n_roots;
	}

	double D = b*b - 4*a*c;

	if (D < 0){
		return 0;
	}
	
	if (D > 0){
		*root_1 = (-b + sqrt(D))/(2.0*a);
		*root_2 = (-b - sqrt(D))/(2.0*a);
		return 2;
	}

	*root_1 = -b/(2.0*a);
	*root_2 = *root_1;
	return 1;
}
