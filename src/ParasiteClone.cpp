//
// Created by thu on 5/9/18.
//

#include <iostream>
#include <cmath>
#include "ParasiteClone.h"
#include "Person.h"
#include "Model.h"
#include "RandomGenerator.h"
#include "Helper.h"
#include "Drug.h"
#include "RandomGeneratorSingleton.h"

ParasiteClone::ParasiteClone(Person *person) : person(person){
    Distribute_To_Bins(ParamNS::DEFAULT_TOTAL_PARASITE_COUNT,
                       ParamNS::DEFAULT_PARASITE_AGE_MEAN,
                       ParamNS::DEFAULT_PARASITE_AGE_STD_DEVIATION); // total 10^11
    Init_Max_Affected_Fraction_Bins();
}

ParasiteClone::ParasiteClone(Person *person,
                             const double &total_parasite_count,
                             const unsigned short &mean_age,
                             const unsigned short &std_deviation) : person(person) {
    Distribute_To_Bins(total_parasite_count, mean_age, std_deviation);
    Init_Max_Affected_Fraction_Bins();
}

void ParasiteClone::Reset_ParasiteClone(Person *person, const double &total_parasite_count, const unsigned short &mean_age,
                                        const unsigned short &std_deviation) {
    if (this->person != person)
        this->person = person;
    Reset_Bins(total_parasite_count, mean_age, std_deviation);
//    Reset_Max_Affected_Fraction_Bins(); # the beta distribution shall be kept unchanged
}

ParasiteClone::~ParasiteClone() {
    Helper::DeleteVector<double>(max_affected_fraction_bins);
    Helper::DeleteVector<double>(parasite_count_bins);
}

// TODO needs optimizing
const double ParasiteClone::Get_Total_Parasite_Count() {
    return (parasite_count_bins.empty()) ? 0 : total_count;
//    if (parasite_count_bins.empty())
//        return 0;
//    else if (!dirty)
//        return total_count;
//    else {
//        total_count = 0.0;
//        for (auto& bin : parasite_count_bins){
////            std::cout << " " << bin << " ";
//            total_count += round(bin);
//        }
//        dirty = false;
//        return total_count;
//    }
//    /*total_count = 0;
//    for (auto& bin : parasite_count_bins){
//        total_count += bin;
//    }
//    return total_count;*/
}

void ParasiteClone::Init_Max_Affected_Fraction_Bins() {
    max_affected_fraction_bins.reserve(ParamNS::MAX_PARASITE_HOUR);
    max_affected_fraction_bins.emplace_back(0); // new-borns are assumed to be new wave of parasites, so the effect of drug is considered to be too little
//    const RandomGenerator *rgen = person->Get_Model()->Get_RandomGenerator();
    const RandomGeneratorSingleton *rgen = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
    for (unsigned short i=1; i<ParamNS::MAX_PARASITE_HOUR; i++){
        max_affected_fraction_bins.emplace_back( rgen->Rand_Beta(ParamNS::MAX_PARASITE_HOUR - i, i) );
    }
}

void ParasiteClone::Reset_Max_Affected_Fraction_Bins() {
//    max_affected_fraction_bins.reserve(ParamNS::MAX_PARASITE_HOUR);
    max_affected_fraction_bins[0]=0.0; // new-borns are assumed to be new wave of parasites, so the effect of drug is considered to be too little
//    const RandomGenerator *rgen = person->Get_Model()->Get_RandomGenerator();
    const RandomGeneratorSingleton *rgen = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
    for (unsigned short i=1; i<ParamNS::MAX_PARASITE_HOUR; i++){
        max_affected_fraction_bins[i] = rgen->Rand_Beta(ParamNS::MAX_PARASITE_HOUR - i, i) ;
    }
}

void ParasiteClone::Distribute_To_Bins(const double &total_parasite_count,
                                       const unsigned short &mean_age,
                                       const unsigned short &std_deviation) {
    total_count = total_parasite_count;
    std::vector<double> age_prob_v; // probability of parasites being distributed into a specific age bin
    age_prob_v.reserve(ParamNS::MAX_PARASITE_HOUR);
    parasite_count_bins = std::vector<double>(ParamNS::MAX_PARASITE_HOUR,0); // initialize 48 bins
//    const RandomGenerator *rgen = person->Get_Model()->Get_RandomGenerator();
    const RandomGeneratorSingleton *rgen = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
    // fill age_prob_v
    for (unsigned short i=0; i<ParamNS::MAX_PARASITE_HOUR; i++){
        age_prob_v.emplace_back(rgen->PDF_Normal(i, mean_age, std_deviation));
    }

    double remain_total = total_parasite_count;
    std::vector<unsigned int> sub_bins(ParamNS::MAX_PARASITE_HOUR,0); // prepare sub bins to draw from multinomial distribution
    while (remain_total > Helper::MAX_UNSIGNED_INT){
//        rgen->Rand_Multinomial(ParamNS::MAX_PARASITE_HOUR, Helper::MAX_UNSIGNED_INT, &age_prob_v.at(0), &sub_bins.at(0));
        rgen->Rand_Multinomial(ParamNS::MAX_PARASITE_HOUR, Helper::MAX_UNSIGNED_INT, &age_prob_v[0], &sub_bins[0]);
        for (unsigned short i=0; i<ParamNS::MAX_PARASITE_HOUR; i++){
//            parasite_count_bins.at(i) += round(sub_bins.at(i));
            parasite_count_bins[i] += round(sub_bins[i]);
        }
        remain_total -= Helper::MAX_UNSIGNED_INT;
    }
//    rgen->Rand_Multinomial(ParamNS::MAX_PARASITE_HOUR, (unsigned int) remain_total, &age_prob_v.at(0), &sub_bins.at(0));
    rgen->Rand_Multinomial(ParamNS::MAX_PARASITE_HOUR, (unsigned int) remain_total, &age_prob_v[0], &sub_bins[0]);
    for (unsigned short i=0; i<ParamNS::MAX_PARASITE_HOUR; i++){
//        parasite_count_bins.at(i) += round(sub_bins.at(i));
        parasite_count_bins[i] += round(sub_bins[i]);
    }

//    dirty = false;

    Helper::DeleteVector<double>(age_prob_v);
    Helper::DeleteVector<unsigned int>(sub_bins);
}

void ParasiteClone::Reset_Bins(const double &total_parasite_count, const unsigned short &mean_age,
                               const unsigned short &std_deviation) {
    total_count = total_parasite_count;
    std::vector<double> age_prob_v(ParamNS::MAX_PARASITE_HOUR,0); // probability of parasites being distributed into a specific age bin
//    const RandomGenerator *rgen = person->Get_Model()->Get_RandomGenerator();
    const RandomGeneratorSingleton *rgen = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
    // fill age_prob_v
    for (unsigned short i=0; i<ParamNS::MAX_PARASITE_HOUR; ++i){
        age_prob_v[i] = rgen->PDF_Normal(i, mean_age, std_deviation);
    }

    double remain_total = total_parasite_count;
    std::vector<unsigned int> sub_bins(ParamNS::MAX_PARASITE_HOUR,0); // prepare sub bins to draw from multinomial distribution
    while (remain_total > Helper::MAX_UNSIGNED_INT){
        rgen->Rand_Multinomial(ParamNS::MAX_PARASITE_HOUR, Helper::MAX_UNSIGNED_INT, &age_prob_v[0], &sub_bins[0]);
        for (unsigned short i=0; i<ParamNS::MAX_PARASITE_HOUR; ++i){
            parasite_count_bins[i] += round(sub_bins[i]);
        }
        remain_total -= Helper::MAX_UNSIGNED_INT;
    }
    rgen->Rand_Multinomial(ParamNS::MAX_PARASITE_HOUR, (unsigned int) remain_total, &age_prob_v[0], &sub_bins[0]);
    for (unsigned short i=0; i<ParamNS::MAX_PARASITE_HOUR; i++){
        parasite_count_bins[i] += round(sub_bins[i]);
    }

    Helper::DeleteVector<double>(age_prob_v);
    Helper::DeleteVector<unsigned int>(sub_bins);
}


// TODO needs optimizing
void ParasiteClone::Killed_By_Drug(const std::vector<Drug*>& drug_v) {
//    std::cout << " " << "total_count " << total_count << " Get_Total " << Get_Total_Parasite_Count() << " ";

    double min_survival = 1.0;
    for (auto& d: drug_v){
        if (d->Get_Current_Concentration() > ParamNS::DEFAULT_DRUG_EFFECTIVE_CONC) {
            double pmax = d->Get_Pmax(), ec50 = d->Get_Ec50(), slope = d->Get_Slope(), conc = d->Get_Current_Concentration();
            min_survival *= 1.0 - pmax * pow(conc, slope) / (pow(conc, slope) + pow(ec50, slope));
        }
    }

    const double old_back = round(parasite_count_bins.back());
    double tmp_total_count = 0.0;
    double tmp_count = 0.0;
    // [] accessor
    for (unsigned short i=ParamNS::MAX_PARASITE_HOUR - 1; i>0; --i){
        double max_affected = max_affected_fraction_bins[i];
        tmp_count = round(parasite_count_bins[i-1] * (max_affected * min_survival + (1.00 -  max_affected)) );
        parasite_count_bins[i] = tmp_count;
        tmp_total_count += tmp_count;
    }
    tmp_count = round(old_back * ParamNS::DEFAULT_PMF);
    parasite_count_bins.front() = tmp_count;
    tmp_total_count += tmp_count;

    total_count = tmp_total_count;


    /*for (auto& d : drug_v){
        if (d->Get_Current_Concentration() > ParamNS::DEFAULT_DRUG_EFFECTIVE_CONC){
            double pmax=d->Get_Pmax(), ec50=d->Get_Ec50(), slope=d->Get_Slope(), conc=d->Get_Current_Concentration();
            double removal = pmax * pow(conc, slope) / ( pow(conc, slope) + pow(ec50, slope)  );
            const double old_back = round(parasite_count_bins.back());

            // [] accessor
            *//*for (unsigned short i=ParamNS::MAX_PARASITE_HOUR - 1; i>0; --i){
//                parasite_count_bins[i] = round(parasite_count_bins[i-1] * (1.0 - max_affected_fraction_bins[i] * removal));
                tmp_count = round(parasite_count_bins[i-1] * (1.0 - max_affected_fraction_bins[i] * removal));
                parasite_count_bins[i] = tmp_count;
                tmp_total_count += tmp_count;
            }*//*

            // using iterators instead
            *//*auto c_i = parasite_count_bins.end();
            auto a_i = max_affected_fraction_bins.end();
            while (c_i > (parasite_count_bins.begin() + 1) ){
                --c_i;
                --a_i;
                double removal = pmax * pow(conc, slope) / ( pow(conc, slope) + pow(ec50, slope)  );
                (*c_i) = round( (*(c_i-1)) * (1.0 - (*a_i)*removal) );
            }*//*

            // using vector::data()
            *//*auto *c_p = parasite_count_bins.data();
            auto *a_p = max_affected_fraction_bins.data();
            for (unsigned short i=ParamNS::MAX_PARASITE_HOUR - 1; i>0; --i){
                double removal = pmax * pow(conc, slope) / ( pow(conc, slope) + pow(ec50, slope)  );
                c_p[i] = round( c_p[i-1] * (1.0 - a_p[i] * removal) );
            }*//*

            // schizogony
//            parasite_count_bins.front() = round(old_back * ParamNS::DEFAULT_PMF);
            tmp_count = round(old_back * ParamNS::DEFAULT_PMF);
            parasite_count_bins.front() = tmp_count;
            tmp_total_count += tmp_count;
            total_count = tmp_total_count;
//            dirty = false;
        }
    }*/


}



void ParasiteClone::Update_ParasiteClone() {
    if (person){
//        if (Get_Total_Parasite_Count() > ParamNS::DEFAULT_PARASITE_EFFECTIVE_COUNT &&
//                Get_Total_Parasite_Count() < ParamNS::DEFAULT_PARASITE_DEADLY_COUNT){
//        if (Get_Total_Parasite_Count() < ParamNS::DEFAULT_PARASITE_DEADLY_COUNT){ // checking here is redundant
//            dirty = true;
            Killed_By_Drug(person->Get_Drug_V());
//        }
    }
    else
        throw std::runtime_error("Null pointer to person");
}



