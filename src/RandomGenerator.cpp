//
// Created by thu on 5/9/18.
//

#include <ctime>
#include <thread>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include "RandomGenerator.h"
#include "Helper.h"

RandomGenerator::RandomGenerator() : seed(Helper::MAX_UNSIGNED_INT), model(nullptr) { // special code to signal true random
    Init_RandomGenerator();
}

RandomGenerator::RandomGenerator(unsigned int& random_seed) : seed(random_seed), model(nullptr){
    Init_RandomGenerator();
}

RandomGenerator::RandomGenerator(Model *model) : seed(Helper::MAX_UNSIGNED_INT), model(model){
    Init_RandomGenerator();
}

RandomGenerator::RandomGenerator(Model *model, unsigned int &random_seed) : seed(random_seed), model(model) {
    Init_RandomGenerator();
}

RandomGenerator::~RandomGenerator() {
    if (rng != nullptr)
        gsl_rng_free(rng);
}

void RandomGenerator::Init_RandomGenerator() {
    if (seed == Helper::MAX_UNSIGNED_INT)
        seed = std::time(NULL) * std::hash<std::thread::id>{}(std::this_thread::get_id())/4294967295; // 4294967295 = max value unsigned int

    rng = gsl_rng_alloc(RNG_T);
    gsl_rng_set(rng, seed);
}

double RandomGenerator::Rand_Uniform(const double &from, const double &to) const {
    return gsl_ran_flat(rng, from, to);
}

double RandomGenerator::Rand_Normal(const double &mean, const double &std_deviation) const {
    return mean + gsl_ran_gaussian(rng, std_deviation);
}

double RandomGenerator::PDF_Normal(const double &value, const double &mean, const double &std_deviation) const {
    return gsl_ran_gaussian_pdf(value-mean, std_deviation);
}


double RandomGenerator::Rand_Beta(const double &a, const double &b) const {
    return gsl_ran_beta(rng, a, b);
}

void RandomGenerator::Rand_Multinomial(const unsigned int &n_draw, const unsigned int &n_total, double *prob_v,
                                       unsigned int *result_v) const {
    gsl_ran_multinomial(rng, n_draw, n_total, prob_v, result_v);
}

double RandomGenerator::PDF_Binomial(const unsigned &k, const unsigned &size, const double &prob) const {
    return gsl_ran_binomial_pdf(k, prob, size);
}

/*void
RandomGenerator::Rand_Multinomial(const unsigned int &n_draw, const unsigned int &n_total, const double &*prob_v,
                                  unsigned int *result_v) {
    gsl_ran_multinomial(rng, n_draw, n_total, prob_v, result_v);
}*/
