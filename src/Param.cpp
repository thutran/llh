//
// Created by thu on 5/8/18.
//

#include <iostream>
#include <gsl/gsl_vector.h>
#include "Param.h"
#include "Helper.h"

using namespace ParamNS;

Param::Param() {
    Init_Default_Param(1);
}

Param::Param(const unsigned short &number_drugs) {
    Init_Default_Param(number_drugs);
}

Param::Param(const std::vector<double> &non_pmax_vector, const std::vector<double> &pmax_vector) {
    if (non_pmax_vector.size() != Size_Non_Pmax_Param_Enum())
        throw std::runtime_error("Mismatch non-pmax parameter set size");
    std::vector<double>().swap(non_pmax_v);
    std::vector<double>().swap(pmax_v);
    for (auto &i : non_pmax_vector)
        non_pmax_v.emplace_back(i);
    for (auto &i : pmax_vector)
        pmax_v.emplace_back(i);
}

Param::~Param() {
    Helper::DeleteVector<double>(non_pmax_v);
    Helper::DeleteVector<double>(pmax_v);
    Helper::DeleteVector<unsigned short>(search_i);
}

void Param::Print() {
    std::cout << "List of non-pmax: " ;
    for(auto& npm : non_pmax_v)
        std::cout << npm << "___";
    std::cout << "; List of pmax: " ;
    for(auto& pm : pmax_v)
        std::cout << pm << "___";
    std::cout << std::endl;
}

const double Param::Get_Sigma() const {
    return non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::SIGMA];
}

const std::vector<double> Param::Get_Pmax_V() const {
    return pmax_v;
}

void Param::Set_Search_I(const std::vector<unsigned short> &search_indices) {
    std::vector<unsigned short>().swap(search_i);
    for (auto &i : search_indices)
        search_i.emplace_back(i);
}

void Param::Absorb_Gsl_Vector(const gsl_vector *x) {
    // check eligible vector
    if (x->size != search_i.size())
        throw std::runtime_error("Mismatch size of search parameter vector");
    unsigned x_i = 0;
    auto pmax_size = pmax_v.size();
    for (auto &i : search_i){
        if (i == (unsigned short)Non_Pmax_Param_Enum::LOG10_PC_MAX)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::LOG10_PC_MAX] = (unsigned short) gsl_vector_get(x, x_i++);
        else if (i == (unsigned short)Non_Pmax_Param_Enum::PA_MEAN)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_MEAN] = (unsigned short) gsl_vector_get(x, x_i++);
        else if (i == (unsigned short)Non_Pmax_Param_Enum::PA_SD)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_SD] = (unsigned short) gsl_vector_get(x, x_i++);
        else if (i == (unsigned short)Non_Pmax_Param_Enum::SIGMA)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::SIGMA] = gsl_vector_get(x, x_i++);
        else if (i > (unsigned short)Non_Pmax_Param_Enum::SIGMA){
            unsigned short tmp_i = i - Size_Non_Pmax_Param_Enum();
            if (tmp_i >= pmax_size)
                throw std::runtime_error("Search parameter index out of bound");
            else
                pmax_v[tmp_i] = gsl_vector_get(x, x_i++);
        }
    }
}

const bool Param::Has_Good_Pmax() const {
    for (auto &i : pmax_v){
        if (i < 0.0 || i > 1.0)
            return false;
    }
    return true;
}

const bool Param::Is_Good() const {
    return (Has_Good_Pmax() &&
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::LOG10_PC_MAX] > 8 &&
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::LOG10_PC_MAX] < 15 &&
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_MEAN] >= 0 &&
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_MEAN] < 48 &&
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_SD] >= 0 &&
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_SD] < 48);
}

void Param::Init_Default_Param(const unsigned short &n_drugs) {
    std::vector<double>().swap(non_pmax_v);
    std::vector<double>().swap(pmax_v);
    non_pmax_v.emplace_back(ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MAX);
    non_pmax_v.emplace_back(ParamNS::DEFAULT_PARASITE_AGE_MEAN);
    non_pmax_v.emplace_back(ParamNS::DEFAULT_PARASITE_AGE_STD_DEVIATION);
    non_pmax_v.emplace_back(ParamNS::DEFAULT_DRUG_SIGMA);
    for (unsigned short i=0; i<n_drugs; ++i)
        pmax_v.emplace_back(ParamNS::DEFAULT_PMAX);
}

const std::vector<double> Param::Get_Non_Pmax_V() const {
    return non_pmax_v;
}

void Param::Replace_Param(const std::vector<unsigned short> &indices, const std::vector<double> &values) {
    if (indices.size() != values.size())
        throw std::runtime_error("Mismatch size of indices and replacing values");

    unsigned v_i = 0;
    auto pmax_size = pmax_v.size();
    for (auto &i : indices){
        if (i == (unsigned short)Non_Pmax_Param_Enum::LOG10_PC_MAX)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::LOG10_PC_MAX] = values[v_i++];
        else if (i == (unsigned short)Non_Pmax_Param_Enum::PA_MEAN)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_MEAN] = values[v_i++];
        else if (i == (unsigned short)Non_Pmax_Param_Enum::PA_SD)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::PA_SD] = values[v_i++];
        else if (i == (unsigned short)Non_Pmax_Param_Enum::SIGMA)
            non_pmax_v[(unsigned short)Non_Pmax_Param_Enum::SIGMA] = values[v_i++];
        else if (i > (unsigned short)Non_Pmax_Param_Enum::SIGMA){
            unsigned short tmp_i = i - Size_Non_Pmax_Param_Enum();
            if (tmp_i >= pmax_size)
                throw std::runtime_error("Replacing parameter index out of bound");
            else
                pmax_v[tmp_i] = values[v_i++];
        }
    }
}

const unsigned short Param::Size_Non_Pmax_Param_Enum() const {
    return ((unsigned short)Non_Pmax_Param_Enum::SIGMA + 1);
}

const double Param::Get_Non_Pmax_Param(const unsigned short &index) const {
    if (index >= Size_Non_Pmax_Param_Enum())
        throw std::runtime_error("Index out of bound");
    else
        return non_pmax_v[index];
}
