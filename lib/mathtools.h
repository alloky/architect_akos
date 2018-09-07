const int MT_INF_ROOTS = -1;

// ax + b = 0

/**
 * @brief      Solve linear equation
 *
 * @param[in]  a       \f$x\f$ coef
 * @param[in]  b       \f$1\f$ coef
 * @param      root    var for root
 * 
 * @return     number of roots
 * 
 * Function for solving this equation \f$ax + b = 0\f$
 * 
 * May return @link MT_INF_ROOTS @endlink if all coefs equal 0.
 * 
 * Example : @include solve_linear_exmpl.cpp
 */
int solve_linear(double a, double b, double* root);


/**
 * @brief      Solve squere equation 
 *
 * @param[in]  a       \f$x^2\f$ coef
 * @param[in]  b       \f$x\f$ coef
 * @param[in]  c       \f$1\f$ coef
 * @param      root_1  First root
 * @param      root_2  Second root
 *
 * @return     number of roots
 *  
 * Function for solving this equation \f$ax^2 + bx + c = 0\f$
 * 
 * May return @link MT_INF_ROOTS @endlink if all coefs equal 0.
 * 
 * Example : @include solve_square_exmpl.cpp
 */
int solve_square(double a, double b, double c, double* root_1, double* root_2);