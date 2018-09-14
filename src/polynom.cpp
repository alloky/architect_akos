#include <sstream> 
#include <assert.h>

#include "polynom.h"
#include "logging.h"

Polynom::Polynom (int n) :
        deg(n),
        coefs(NULL),
        roots(NULL) {
    assert(n >= 1);
}

int Polynom::find_roots(){
    if(deg == 1){
        return _find_roots_linear();    
    }
    
    if(deg == 2){
        return _find_roots_square();
    }

    return MT_NOT_IMPL; //_find_roots_newthon();

}


int Polynom::_find_roots_linear(){
    double root = NAN;
    int n_roots = solve_linear(coefs[1], coefs[0], &root);
    switch(n_roots){
        case 0:
            return 0;
       
        case 1:
            roots = new double[1];
            roots_muls = new char[1];
            roots[0] = root;
            roots_muls[0] = 1;
            return 1;
       
        case MT_INF_ROOTS:
            return MT_INF_ROOTS;

         default:
            LEV_LOG(LL_ERR, "mathtools.solve_linear return unknown number of roots");
            return MT_NOT_IMPL;
    }
}


int Polynom::_find_roots_square(){
    double root_1 = NAN,
           root_2 = NAN;

    int n_roots = solve_square(coefs[2], coefs[1], coefs[0], &root_1, &root_2);
    switch(n_roots){
        case 0:
            return 0;

        case 1:
            roots = new double[1];
            roots_muls= new char[1];
            roots[0] = root_1;
            roots_muls[0] = 2;
            return 1;
       
        case 2:
            roots = new double[2];
            roots_muls = new char[2];
            roots[0] = root_1;
            roots[1] = root_2;
            roots_muls[0] = 1;
            roots_muls[1] = 1;
            return 2;
       
        case MT_INF_ROOTS:
            return MT_INF_ROOTS;

        default:
            LEV_LOG(LL_ERR, "mathtools.solve_square return unknown number of roots");
            return MT_NOT_IMPL;
    }
}

void Polynom::parse_coefs(const std::string& space_sep_coefs){
    
    std::istringstream stream(space_sep_coefs);
    coefs = new double[deg + 1];

    assert(coefs != NULL);

    for(int i = 0; i < deg + 1; ++i){
        double cur_coef = NAN;
        stream >> cur_coef;
        coefs[i] = cur_coef;  
    }
}

Polynom::~Polynom(){
    if(coefs != NULL)
    	delete coefs;
    if(roots_muls != NULL)
    	delete roots_muls;
    if(roots_muls != NULL)
	   delete roots;
}
