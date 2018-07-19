//
// Created by thu on 7/17/18.
//

#include <iostream>
#include "RandomGeneratorSingleton.h"
#include "Helper.h"
#include "RandomGenerator.h"

RandomGeneratorSingleton::~RandomGeneratorSingleton() {}

RandomGeneratorSingleton *RandomGeneratorSingleton::Get_RandomGeneratorSingleton(const unsigned int &seed) {
    static RandomGeneratorSingleton rg_singleton(seed);
//    std::cout << "seed: " << rg_singleton.seed << std::endl;
    return &rg_singleton;
}


RandomGeneratorSingleton *RandomGeneratorSingleton::Get_RandomGeneratorSingleton() {
    return Get_RandomGeneratorSingleton(Helper::MAX_UNSIGNED_INT);
}

RandomGeneratorSingleton::RandomGeneratorSingleton(const unsigned int &seed) : seed(seed) {
//    std::cout << "RanGenSingleton constructor: " << this << std::endl;
    static RandomGenerator rgen(seed);
    this->rg = & rgen;
}

double RandomGeneratorSingleton::Rand_Uniform(const double &from, const double &to) const {
    return Get_RandomGeneratorSingleton()->rg->Rand_Uniform(from, to);
}

double RandomGeneratorSingleton::Rand_Normal(const double &mean, const double &std_deviation) const {
    return Get_RandomGeneratorSingleton()->rg->Rand_Normal(mean, std_deviation);
}

double
RandomGeneratorSingleton::PDF_Normal(const double &value, const double &mean, const double &std_deviation) const {
    return Get_RandomGeneratorSingleton()->rg->PDF_Normal(value, mean, std_deviation);
}

double RandomGeneratorSingleton::Rand_Beta(const double &a, const double &b) const {
    return Get_RandomGeneratorSingleton()->rg->Rand_Beta(a, b);
}

void RandomGeneratorSingleton::Rand_Multinomial(const unsigned int &n_draw, const unsigned int &n_total, double *prob_v,
                                                unsigned int *result_v) const {
    Get_RandomGeneratorSingleton()->rg->Rand_Multinomial(n_draw, n_total, prob_v, result_v);
}

double RandomGeneratorSingleton::PDF_Binomial(const unsigned &k, const unsigned &size, const double &prob) const {
    return Get_RandomGeneratorSingleton()->rg->PDF_Binomial(k, size, prob);
}

double RandomGeneratorSingleton::Rand_Uniform_Pseudo() {
    if (r_unif_pseu.empty())
        throw std::runtime_error("Call Init_Rand_Uniform_Pseudo first");
    if (r_unif_pseu_next_index == r_unif_pseu.size()){
        r_unif_pseu_next_index = 0;
    }
    return r_unif_pseu[r_unif_pseu_next_index++];
}

void RandomGeneratorSingleton::Init_Rand_Uniform_Pseudo(const double &from, const double &to, const double &step) {
    if (from > to)
        throw std::runtime_error("Reversed bounds");
    if (!r_unif_pseu.empty())
        Helper::DeleteVector<double >(r_unif_pseu);

    double tmp = from;
    while (tmp <= to){
        r_unif_pseu.emplace_back(tmp);
        tmp += step;
    }
    r_unif_pseu_next_index = 0;
}
