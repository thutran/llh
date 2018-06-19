//
// Created by thu on 5/8/18.
//

#ifndef LIKELIHOOD_PARAM_H
#define LIKELIHOOD_PARAM_H

#include <vector>
#include <gsl/gsl_vector.h>

namespace ParamNS{
    const double DEFAULT_PMAX = 0.99;
    const unsigned short DEFAULT_PMF = 1; // maybe the scenario we're trying to simulate is after the growth phase of the parasite
    const unsigned short MAX_PARASITE_HOUR = 48;
    const unsigned long long DEFAULT_TOTAL_PARASITE_COUNT = 100000000000; // 10^11
//    const unsigned long long DEFAULT_TOTAL_PARASITE_COUNT = 10000000000; // 10^10
    const unsigned short DEFAULT_LOG10_PARASITE_COUNT_MIN = 8;
    const unsigned short DEFAULT_LOG10_PARASITE_COUNT_MAX = 10;
    const unsigned long long DEFAULT_PARASITE_DEADLY_COUNT = 1000000000000000; // 10^15
    const unsigned short DEFAULT_PARASITE_AGE_MEAN = 26;
//    const unsigned short DEFAULT_PARASITE_AGE_MEAN_MIN = 0;
//    const unsigned short DEFAULT_PARASITE_AGE_MEAN_MAX = 48;
    const unsigned short DEFAULT_PARASITE_AGE_STD_DEVIATION = 12;
    const unsigned int DEFAULT_PARASITE_EFFECTIVE_COUNT = 100;
    const unsigned int DEFAULT_PARASITE_DETECTION_COUNT = 100000000;
    const double DEFAULT_DRUG_SIGMA = 0.2; // absorption sigma
    const double DEFAULT_DRUG_EFFECTIVE_CONC = 0.001;
    const double DEFAULT_EC50_MIN = 0.6;
    const double DEFAULT_EC50_MAX = 1.4;
    const unsigned short DEFAULT_DRUG_SLOPE_MIN = 15;
    const unsigned short DEFAULT_DRUG_SLOPE_MAX = 25;

    enum class Non_Pmax_Param_Enum{
        LOG10_PC_MAX, // 0
        PA_MEAN, // 1
        PA_SD, // 2
        SIGMA, // 3
        SIZE
    };

    class Param {
    public:
        std::vector<double> pmax_v; // vector of pmax for each antimalarial in the combination treatment
        double sigma; // representation of variation in drug absorption
        std::vector<unsigned short> search_i;
        unsigned short log10_parasite_count_max = DEFAULT_LOG10_PARASITE_COUNT_MAX;
        unsigned short parasite_age_mean = DEFAULT_PARASITE_AGE_MEAN;
        unsigned short parasite_age_std_deviation = DEFAULT_PARASITE_AGE_STD_DEVIATION;
//        unsigned short pmf=10; // parasite multiplicative factor

        Param();
        Param(const double& drug_absorption_sigma, const std::vector<double>& pmax_vector);
        Param(const unsigned short &log10_parasite_count_max, const unsigned short &parasite_age_mean,
              const unsigned short &parasite_age_sd, const double &drug_absorption_sigma,
              const std::vector<double> &pmax_vector);
        ~Param();

        void Print();
        const double Get_Sigma() const ;
        const std::vector<double> Get_Pmax_V() const ;
        void Set_Search_I(const std::vector<unsigned short> &search_indices);
        void Set_Param_I(const std::vector<unsigned short> &param_indices);
        void Absorb_Gsl_Vector(const gsl_vector *x);
        const bool Is_Good() const ;
        const bool Has_Good_Pmax() const ;
    };

}

#endif //LIKELIHOOD_PARAM_H
