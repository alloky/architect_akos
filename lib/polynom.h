#include <string> 

#include "mathtools.h"


/**
 * @brief      Class for polynoms.
 *
 *             Class for work with polynomials with double coefs.
 *
 *             Supports root finding and spaces-divide string parsing.
 */
class Polynom {
public:	
	/**
	 * @brief degree of polynomial
	 */
	int deg;
	
	/**
	 * @brief coefs of polynomial
	 */
	double* coefs;
	
	/**
	 * @brief roots of polynomial
	 * 
	 * Will be allocated and filled in @link find_roots @endlink
	 */
	double* roots;
	
	/**
	 * @brief multiplicities of roots of the polynomial
	 * 
	 * Will be allocated and filled in @link find_roots @endlink
	 */
	char* roots_muls;

	/**
	 * @brief      Constructs the polynom with deg == n
	 *
	 * @param[in]  n     degree
	 */
	Polynom(int n);

	
	/**
	 * @brief      Parse polynom coefs from string
	 *
	 * @param      s     string with coef separated with spaces
	 */
	void parse_coefs(const std::string& space_sep_coefs);
	
	/**
	 * @brief      Find all roots of this polynom
	 *
	 * @return     number of roots
	 * 
	 * Roots stored in new allocated array in @link roots @endlink field.
	 * Roots' multiplicities stored in @link roots_muls  @endlink
	 * May return @link MT_NOT_IMPL @endlink if deg >= 3
	 * 
	 */
	int find_roots();

	/**
	 * @brief      Destroys the object and frees all arrays
	 */
	~Polynom();
private:
	int _find_roots_linear();
	int _find_roots_square();
};