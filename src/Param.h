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
    const double DEFAULT_TOTAL_PARASITE_COUNT = 1e11;
    const unsigned short DEFAULT_LOG10_PARASITE_COUNT_MIN = 8;
    const unsigned short DEFAULT_LOG10_PARASITE_COUNT_MAX = 12;
    const double DEFAULT_PARASITE_DEADLY_COUNT = 1e15;
    const unsigned short DEFAULT_PARASITE_AGE_MEAN = 26;
    const unsigned short DEFAULT_PARASITE_AGE_STD_DEVIATION = 12;
    const unsigned DEFAULT_PARASITE_EFFECTIVE_COUNT = 100;
    const unsigned DEFAULT_PARASITE_DETECTION_COUNT = 100000000; // 10^8
    const double DEFAULT_DRUG_SIGMA = 0.4;//0.4; // absorption sigma
    const double DEFAULT_DRUG_EFFECTIVE_CONC = 0.001;
    const double DEFAULT_EC50_MIN = 0.6;//0.6;
    const double DEFAULT_EC50_MAX = 1.4;
    const unsigned short DEFAULT_DRUG_SLOPE_MIN = 15;//15;
    const unsigned short DEFAULT_DRUG_SLOPE_MAX = 25;
    const unsigned RANDOM_SEED = 1111;

    enum class Non_Pmax_Param_Enum : unsigned short{
        LOG10_PC_MAX, // 0
        PA_MEAN, // 1
        PA_SD, // 2
        SIGMA, // 3
//        EC50,
//        SLOPE,
        SIZE
    };

    class Param {
    public:

        Param();
        explicit Param(const unsigned short &number_drugs);
        Param(const std::vector<double> &non_pmax_vector,
              const std::vector<double> &pmax_vector);
        ~Param();

        void Print();
        const double Get_Sigma() const ;
        const std::vector<double> Get_Pmax_V() const ;
        const std::vector<double> Get_Non_Pmax_V() const ;
        void Init_Default_Param(const unsigned short &n_drugs);
        void Set_Search_I(const std::vector<unsigned short> &search_indices);
        void Absorb_Gsl_Vector(const gsl_vector *x);
        void Replace_Param(const std::vector<unsigned short> &indices, const std::vector<double> &values);
        const bool Is_Good() const ;
        const bool Has_Good_Pmax() const ;
        const double Get_Non_Pmax_Param(const unsigned short &index) const ;

    private:
        std::vector<double> non_pmax_v;
        std::vector<double> pmax_v; // vector of pmax for each antimalarial in the combination treatment
        std::vector<unsigned short> search_i; // indices of param to be optimized
//        unsigned short pmf=10; // parasite multiplicative factor
    };

}

#endif //LIKELIHOOD_PARAM_H
