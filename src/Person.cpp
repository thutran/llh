//
// Created by thu on 5/9/18.
//

#include <cmath>
#include <stdexcept>
#include <iostream>

#include "Model.h"
#include "Person.h"
#include "ParasiteClone.h"
#include "Helper.h"
#include "Param.h"
#include "RandomGenerator.h"
#include "Trial.h"
#include "Drug.h"

Person::Person(Model *model) : model(model) {
    Init_Person();
}

/*Person::Person(Model *model, const long long &total_parasite_count, const unsigned short &mean_age,
               const unsigned short &parasite_count_sd, const double &drug_conc_sigma) : model(model), drug_absorption_sigma(drug_conc_sigma) {
    Init_Person();
    Init_Parasite_Clone(total_parasite_count, mean_age, parasite_count_sd);
}*/

Person::~Person() {
    Helper::DeletePointer<ParasiteClone>(parsite_clone);
    Helper::DeleteVector<Drug*>(drug_v);
}

void Person::Init_Person() {
    new_dose_hour_v = model->Get_Trial()->Get_New_Dose_Hour_V();
    // parasite population
    Init_Parasite_Clone();
    // drug vector
    Init_Drug_V();
    updatable = true;
}

void Person::Init_Parasite_Clone() {
    const RandomGenerator *rgen = model->Get_RandomGenerator();
//    unsigned long long total = (unsigned long long)(rgen->Rand_Uniform(1,9) * pow(10, rgen->Rand_Uniform(ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MIN, ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MAX)));
    double total = rgen->Rand_Uniform(1,9) * pow(10, rgen->Rand_Uniform(ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MIN, ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MAX));
//    unsigned long long total = (unsigned long long) ((double)ParamNS::DEFAULT_TOTAL_PARASITE_COUNT * pow(10, rgen->Rand_Normal(0,1)));
    unsigned short mean_age = (unsigned short)rgen->Rand_Uniform(ParamNS::DEFAULT_PARASITE_AGE_MEAN - 10, ParamNS::DEFAULT_PARASITE_AGE_MEAN + 10);
    unsigned short sd_age = (unsigned short)rgen->Rand_Uniform(ParamNS::DEFAULT_PARASITE_AGE_STD_DEVIATION - 5, ParamNS::DEFAULT_PARASITE_AGE_STD_DEVIATION + 5);
//    std::cout << mean_age << " " << sd_age << std::endl;
//    std::cout << " total: " << total << " ";
    parsite_clone = new ParasiteClone(this, total, mean_age , sd_age);
}

void Person::Init_Drug_V() {
    const RandomGenerator *rgen = model->Get_RandomGenerator();
    const double drug_sigma = model->Get_Param_Set()->Get_Sigma();
    const std::vector<Drug*> trial_drug_v = model->Get_Trial()->Get_Drug_V();
    const std::vector<double> pmax_v = model->Get_Param_Set()->Get_Pmax_V();
    if (trial_drug_v.size() != pmax_v.size())
        throw std::runtime_error("Mismatch length pmax parameters and drugs");
    for (unsigned short i=0; i<pmax_v.size(); i++){
        double init_conc = rgen->Rand_Uniform(1.0 - drug_sigma, 1.0 + drug_sigma);
        double ec50 = rgen->Rand_Uniform(ParamNS::DEFAULT_EC50_MIN, ParamNS::DEFAULT_EC50_MAX);
        unsigned short slope = (unsigned short)rgen->Rand_Uniform(ParamNS::DEFAULT_DRUG_SLOPE_MIN, ParamNS::DEFAULT_DRUG_SLOPE_MAX);
        drug_v.emplace_back(new Drug(init_conc, trial_drug_v.at(i)->Get_Halflife(),
                                  pmax_v.at(i), ec50, slope));
    }
}

void Person::Init_Parasite_Clone(const double &total_parasite_count, const unsigned short &mean_age,
                                 const unsigned short &parasite_count_sd) {
    parsite_clone = new ParasiteClone(this, total_parasite_count, mean_age, parasite_count_sd);
}

const Model *Person::Get_Model() const {
    if (model)
        return model;
    else
        throw std::runtime_error("Null pointer to model");
}

const std::vector<Drug *> Person::Get_Drug_V() const {
    return drug_v;
}

void Person::Update_Person() {
    const unsigned int t = model->Get_Timer();
    double total_parasite_count = parsite_clone->Get_Total_Parasite_Count();
    bool has_drug = false;
    bool dead = total_parasite_count > ParamNS::DEFAULT_PARASITE_DEADLY_COUNT;
    bool has_parasite = false;
//    std::cout << t << " " << parsite_clone->Get_Total_Parasite_Count() << " ";
    if (!dead){
        bool new_dose = false;
        for(auto& ndh : new_dose_hour_v){
            if (t == ndh){
                new_dose = true;
                break;
            }
        }
        // update drug
        for (auto& d : drug_v){
            if (new_dose || d->Get_Current_Concentration() > ParamNS::DEFAULT_DRUG_EFFECTIVE_CONC ){
                d->Update_Concentration(new_dose);
                has_drug = true;
            }
//            std::cout << d->Get_Current_Concentration() << " ";
        }
//        std::cout << std::endl;
        // update parasite clone
        if (has_drug || total_parasite_count > ParamNS::DEFAULT_PARASITE_EFFECTIVE_COUNT){
            parsite_clone->Update_ParasiteClone();
            has_parasite = true;
        }
    }

    updatable = has_drug && has_parasite && (!dead);
}

const bool Person::Is_Cured() const {
//    return ((double)ParamNS::DEFAULT_PARASITE_DETECTION_COUNT - (double)parsite_clone->Get_Total_Parasite_Count() > 0.0);
    return ((double)ParamNS::DEFAULT_PARASITE_DETECTION_COUNT - parsite_clone->Get_Total_Parasite_Count() > 0.0);
}

const bool Person::Is_Updatable() const {
    return updatable;
}




