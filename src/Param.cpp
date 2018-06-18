//
// Created by thu on 5/8/18.
//

#include <iostream>
#include <gsl/gsl_vector.h>
#include "Param.h"
using namespace ParamNS;

Param::Param() {}

Param::~Param() {}

Param::Param(const std::vector<double> &pmax_vector, const double &sigma) : pmax_v(pmax_vector), sigma(sigma) {
}

void Param::Print() {
    std::cout << "List of pmax: " ;
    for(auto& pm : pmax_v){
        std::cout << pm << " ";
    }
    std::cout << std::endl << "Sigma: " << sigma << std::endl;
}

const double Param::Get_Sigma() const {
    return sigma;
}

const std::vector<double> Param::Get_Pmax_V() const {
    return pmax_v;
}

void Param::Absorb_Gsl_Vector(const gsl_vector *x) {
    auto x_size = x->size;
    sigma = gsl_vector_get(x, --x_size);
    std::vector<double>().swap(pmax_v);
    for (unsigned i=0; i<x_size; ++i){
        pmax_v.emplace_back(gsl_vector_get(x, i));
    }
}
