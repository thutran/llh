//
// Created by thu on 5/8/18.
//

#include <iostream>
#include <gsl/gsl_vector.h>
#include "Param.h"
using namespace ParamNS;

Param::Param() {}

Param::~Param() {}

Param::Param(const double &drug_absorption_sigma,
             const std::vector<double> &pmax_vector)
        : sigma(drug_absorption_sigma),
          pmax_v(pmax_vector){
}

Param::Param(const unsigned short &log10_parasite_count_max,
             const unsigned short &parasite_age_mean,
             const unsigned short &parasite_age_sd,
             const double &drug_absorption_sigma,
             const std::vector<double> &pmax_vector)
        : log10_parasite_count_max(log10_parasite_count_max),
          parasite_age_mean(parasite_age_mean),
          parasite_age_std_deviation(parasite_age_sd),
          sigma(drug_absorption_sigma),
          pmax_v(pmax_vector) {
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

void Param::Set_Search_I(const std::vector<unsigned short> &search_indices) {
    search_i = search_indices;
}

void Param::Absorb_Gsl_Vector(const gsl_vector *x) {
//    auto x_size = x->size;
//    sigma = gsl_vector_get(x, --x_size);
//    std::vector<double>().swap(pmax_v);
//    for (unsigned i=0; i<x_size; ++i){
//        pmax_v.emplace_back(gsl_vector_get(x, i));
//    }

    // check eligible vector
    if (x->size != search_i.size())
        throw std::runtime_error("Mismatch size of search parameter vector");

    unsigned x_i = 0;
    auto pmax_size = pmax_v.size();
    for (auto i : search_i){
        if (i == (unsigned short)Non_Pmax_Param_Enum::LOG10_PC_MAX)
            log10_parasite_count_max = (unsigned short)gsl_vector_get(x, x_i++);
        else if (i == (unsigned short)Non_Pmax_Param_Enum::PA_MEAN)
            parasite_age_mean = (unsigned short)gsl_vector_get(x, x_i++);
        else if (i == (unsigned short)Non_Pmax_Param_Enum::PA_SD)
            parasite_age_std_deviation = (unsigned short)gsl_vector_get(x, x_i++);
        else if (i == (unsigned short)Non_Pmax_Param_Enum::SIGMA)
            sigma = gsl_vector_get(x, x_i++);
        else if (i >= (unsigned short)Non_Pmax_Param_Enum::SIZE){
            unsigned short tmp_i = i - (unsigned short)Non_Pmax_Param_Enum::SIZE;
            if (tmp_i >= pmax_size)
                throw std::runtime_error("Search parameter index out of bound");
            else
                pmax_v[tmp_i] = gsl_vector_get(x, x_i++);
        }

//        if (i >= pmax_size)
//            sigma = gsl_vector_get(x, x_i++);
//        else
//            pmax_v[i] = gsl_vector_get(x, x_i++);
    }
}

const bool Param::Has_Good_Pmax() const {
    for (auto i : pmax_v){
        if (i < 0.0 || i > 1.0)
            return false;
    }
    return true;
}

const bool Param::Is_Good() const {
    return (Has_Good_Pmax() &&
            log10_parasite_count_max > 8 && log10_parasite_count_max < 15 &&
            parasite_age_mean >= 0 && parasite_age_mean < 48 &&
            parasite_age_std_deviation >= 0 && parasite_age_std_deviation < 48);
//    return false;
}

void Param::Set_Param_I(const std::vector<unsigned short> &param_indices) {

}
