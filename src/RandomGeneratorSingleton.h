//
// Created by thu on 7/17/18.
//

#ifndef LIKELIHOOD_RANDOMGENERATORSINGLETON_H
#define LIKELIHOOD_RANDOMGENERATORSINGLETON_H

//#include "RandomGenerator.h"

#include <vector>

class RandomGenerator;

class RandomGeneratorSingleton {
public:
    static RandomGeneratorSingleton *Get_RandomGeneratorSingleton(const unsigned int &seed);
    static RandomGeneratorSingleton *Get_RandomGeneratorSingleton();

    ~RandomGeneratorSingleton();

    double Rand_Uniform(const double& from, const double& to) const;
    double Rand_Normal(const double& mean, const double& std_deviation) const ;
    double PDF_Normal(const double& value, const double& mean, const double& std_deviation) const ;
    double Rand_Beta(const double& a, const double& b) const ;
    void Rand_Multinomial(const unsigned int &n_draw, const unsigned int &n_total, double prob_v[],
                          unsigned int result_v[]) const ;
    double PDF_Binomial(const unsigned& k, const unsigned& size, const double& prob) const ;
    double Rand_Uniform_Pseudo();
    void Init_Rand_Uniform_Pseudo(const double& from, const double& to, const double& step);
private:
    unsigned int seed;
    RandomGenerator *rg;
    std::vector<double> r_unif_pseu;
    unsigned int r_unif_pseu_next_index;

    explicit RandomGeneratorSingleton(const unsigned int & seed);

    // not defined, prevent copying
    RandomGeneratorSingleton(const RandomGeneratorSingleton& other);
    RandomGeneratorSingleton& operator= (const RandomGeneratorSingleton& other);
};


#endif //LIKELIHOOD_RANDOMGENERATORSINGLETON_H
