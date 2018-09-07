#include "mathtools.h"


/**
 * @brief      Class for polynoms.
 *
 *             Class for work with polynomials with double coefs.
 *
 *             Supports root finding and spaces-divide string parsing.
 */
class Polynom{
public:	
	int deg;
	double* coefs;
	
	double* roots;
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
	void parse_coefs(std::string& s);
	
	/**
	 * @brief      Find all roots of this polynom
	 *
	 * @return     number of roots
	 * 
	 * Roots stored in new allocated array in @link roots @endlink field.
	 * Roots' multiplicities stored in @link roots_muls  @endlink
	 * 
	 */
	int find_roots();
	~Polynom();
private:
	int _find_roots_linear();
	int _find_roots_square();
};