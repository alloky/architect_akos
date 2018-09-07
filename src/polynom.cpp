#include <sstream> 
#include <assert>

#include "polynom.h"

Polynom::Polynom (int n) :
        deg(n),
        coefs(NULL),
        roots(NULL) {
}

int Polynom::find_roots(){
    if(deg == 1){
        return _find_roots_linear();    
    }
    
    if(deg == 2){
        return _find_roots_square();
    }

    return _find_roots_newthon();

}


int Polynom::_find_roots_linear(){
    double root = 0;
    int n_roots = solve_linear(coefs[1], coefs[0], &root);
    switch(n_roots){
        case 0:
            return 0;
        case 1:
            roots = new double[1];
            chars = new char[1];
            roots[0] = root;
            chars[0] = 1;
            return 1;
        case MT_INF_ROOTS:
            return MT_INF_ROOTS;
    }
}


int Polynom::_find_roots_square(){
    double root = 0.0,
           root_2 = 0.0;

    int n_roots = solve_square(coefs[2], coefs[1], coefs[0], &root_1, &root_2);
    switch(n_roots){
        case 0:
            return 0;
        case 1:
            roots = new double[1];
            chars = new int[1];
            roots[0] = root_1;
            chars[0] = 2;
            return 1;
        case 2:
            roots = new double[2];
            chars = new int[2]
            roots[0] = root_1;
            roots[1] = root_2;
            chars[0] = 1;
            chars[1] = 1;
            return 2;
        case MT_INF_ROOTS:
            return MT_INF_ROOTS;
    }
}

void Polynom::parse_coefs(const std::string& space_sep_coefs){
    
    std::istringstream stream(space_sep_coefs);
    coefs = new double[n];

    assert(coefs != NULL);
    
    for(int i = 0; i < n; ++i){
        double cur_coef = 0.0;
        stream >> cur_coef;
        coefs[i] = cur_coef;  
    }
}

Polynom::~Polynom(){
    delete coefs;
    delete chars;
    delete roots;
}