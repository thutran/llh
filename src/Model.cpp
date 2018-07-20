//
// Created by thu on 5/8/18.
//

#include <iostream>
#include <cmath>

#include "Model.h"
#include "Param.h"
#include "Person.h"
#include "RandomGenerator.h"
#include "Trial.h"
#include "Helper.h"
#include "RandomGeneratorSingleton.h"
//#include <gsl/gsl_rng.h>

Model::Model(const unsigned int &pop_size, ParamNS::Param *param_set) : n_total(pop_size), param(param_set) {
    n_hour = Helper::DEFAULT_RUNTIME;
    Init_Model();
    Init_Person_V();
}

Model::Model(const unsigned int &pop_size, const unsigned int &total_days, ParamNS::Param *param_set) : n_total(pop_size), n_hour(total_days*24), param(param_set) {
    Init_Model();
    Init_Person_V();
}

Model::Model(Trial *trial, ParamNS::Param *param_set) : trial(trial), param(param_set){
    n_total = Helper::DEFAULT_POP_SIZE; //trial->Get_Pop_Size() * scale_up_n_total;
    n_hour = Helper::DEFAULT_RUNTIME;
    Init_Model();
    Init_Person_V();
}

Model::~Model() {
//    Helper::DeletePointer(param);
//    Helper::DeletePointer(Trial);
    Helper::DeleteVector<Person*>(updatable_person_v);
    Helper::DeleteVector<Person*>(person_v);
}

void Model::Reset_Model(Trial *trial, ParamNS::Param *param_set) {
    this->trial = trial;
    this->param = param_set;
    timer = 0;
    Reset_Person_V();
}

void Model::Init_Model() {
    rand_gen_singleton = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
    timer = 0;
    person_v.reserve(n_total);

    // pseudo uniform random for log_10_total_parasite_count
    rand_gen_singleton->Init_Rand_Uniform_Pseudo(8.5, 11.5, 0.5);
}

void Model::Init_Person_V() {
    for (unsigned int i=0; i<n_total; i++){
        // draw initial parasite count
        const auto &prm_log10_pc_max = param->Get_Non_Pmax_Param((unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX);
        double total = rand_gen_singleton->Rand_Uniform(1,9) * pow(10,
                                                         rand_gen_singleton->Rand_Uniform(ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MIN, prm_log10_pc_max));
//        double total = pow(10, rand_gen_singleton->Rand_Uniform_Pseudo() );
        unsigned short mean_age = (unsigned short)rand_gen_singleton->Rand_Uniform(0, ParamNS::MAX_PARASITE_HOUR - 1);
        unsigned short sd_age = (unsigned short)rand_gen_singleton->Rand_Uniform(0, ParamNS::MAX_PARASITE_HOUR - 1);

//    const auto &prm_pa_mean = param->Get_Non_Pmax_Param((unsigned short)ParamNS::Non_Pmax_Param_Enum::PA_MEAN);
//    const auto &prm_pa_sd = param->Get_Non_Pmax_Param((unsigned short)ParamNS::Non_Pmax_Param_Enum::PA_SD);
//    unsigned short mean_age = (unsigned short)rand_gen->Rand_Normal(prm_pa_mean, 5);
//    unsigned short sd_age = (unsigned short)rand_gen->Rand_Normal(prm_pa_sd, 3);

        person_v.emplace_back(new Person(this, total, mean_age, sd_age));
        // draw initial absorbed drug concentration
    }
    updatable_person_v = person_v;
}

void Model::Reset_Person_V() {
    for (unsigned int i=0; i<n_total; i++){
        // draw initial parasite count
        const auto &prm_log10_pc_max = param->Get_Non_Pmax_Param((unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX);
        double total = rand_gen_singleton->Rand_Uniform(1,9) * pow(10,
                                                         rand_gen_singleton->Rand_Uniform(ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MIN, prm_log10_pc_max));
//        double total = pow(10, rand_gen_singleton->Rand_Uniform_Pseudo() );
        unsigned short mean_age = (unsigned short)rand_gen_singleton->Rand_Uniform(0, ParamNS::MAX_PARASITE_HOUR - 1);
        unsigned short sd_age = (unsigned short)rand_gen_singleton->Rand_Uniform(0, ParamNS::MAX_PARASITE_HOUR - 1);

        person_v[i]->Reset_Person(this, total, mean_age, sd_age);
        // draw initial absorbed drug concentration
    }
    updatable_person_v = person_v;
}

void Model::Run() {
    while (timer <= n_hour){
        Tick();
    }
}

void Model::Tick() {
    unsigned int not_updatable = 0;
    std::vector<int> rm_i_v;
    /*for (auto& p : person_v){
        if (p->Is_Updatable())
            p->Update_Person();
        else
            not_updatable += 1;
    }*/
//    for(auto it = updatable_person_v.end(); it > updatable_person_v.begin(); --it){
    auto it = updatable_person_v.end();
    while (it > updatable_person_v.begin()){
        --it;
        if ((*it)->Is_Updatable())
            (*it)->Update_Person();
        else {
            not_updatable += 1;
            rm_i_v.emplace_back( it - updatable_person_v.begin() );
        }
    }
    for (auto& ri : rm_i_v)
        Helper::RemoveFromVector(updatable_person_v, updatable_person_v.begin() + ri);
//    if (timer % 24 == 0)
//        std::cout << timer << " " << Get_Cure_Number() << std::endl;
    // stop if there is no more updatable person
    not_updatable == n_total ? (timer = n_hour + 10) : (timer += 1);

}

const ParamNS::Param *Model::Get_Param_Set() const {
    if (param)
        return param;
    else
        throw std::runtime_error("Null pointer to parameters set");
}

const Trial *Model::Get_Trial() const {
    if (trial)
        return trial;
    else
        throw std::runtime_error("Null pointer to trial");
}

const unsigned int Model::Get_Timer() const {
    return timer;
}

const unsigned int Model::Get_Cure_Number() const {
    unsigned int sum = 0;
    for (auto& p : person_v){
        sum += (p->Is_Cured()) ? 1 : 0;
    }
    return sum;
}

const double Model::Calculate_Negative_Log_Likelihood()  {
    unsigned sim_cured = Get_Cure_Number();
    unsigned trial_cured = trial->Get_Cured();
    unsigned trial_total = trial->Get_Pop_Size();
    double prob = (double)sim_cured / (double)n_total;
//    negative_log_likelihood = -(log( rand_gen_singleton->PDF_Binomial(trial_cured, trial_total, prob) + Helper::LOG_CONST));
    negative_log_likelihood = -(log(pow(prob, trial_cured) + Helper::LOG_CONST) + log(pow(1.0 - prob, trial_total - trial_cured) + Helper::LOG_CONST));
    return negative_log_likelihood;
}

const double Model::Get_Negative_Log_Likelihood() const {
    return negative_log_likelihood;
}

RandomGeneratorSingleton *Model::Get_RandomGeneratorSingleton() const {
    return rand_gen_singleton;
}

