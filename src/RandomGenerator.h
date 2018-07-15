//
// Created by thu on 5/9/18.
//

#ifndef LIKELIHOOD_RANDOMGENERATOR_H
#define LIKELIHOOD_RANDOMGENERATOR_H

#include <gsl/gsl_rng.h>

class Model;

class RandomGenerator {
public:
    const gsl_rng_type * RNG_T = gsl_rng_mt19937; // type of random number generator
    gsl_rng *rng;
    unsigned int seed;
    Model *model;

    Model *Get_Model() const;

    RandomGenerator();
    explicit RandomGenerator(const unsigned int &random_seed);
    explicit RandomGenerator(Model* model);
    RandomGenerator(Model *model, const unsigned int &random_seed);

    ~RandomGenerator();

    void Init_RandomGenerator();

    /* necessary random generators */
    double Rand_Uniform(const double& from, const double& to) const;
    double Rand_Normal(const double& mean, const double& std_deviation) const ;
    double PDF_Normal(const double& value, const double& mean, const double& std_deviation) const ;
    double Rand_Beta(const double& a, const double& b) const ;
    void Rand_Multinomial(const unsigned int &n_draw, const unsigned int &n_total, double prob_v[],
                          unsigned int result_v[]) const ;
    double PDF_Binomial(const unsigned& k, const unsigned& size, const double& prob) const ;
};


#endif //LIKELIHOOD_RANDOMGENERATOR_H
