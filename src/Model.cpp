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
//#include <gsl/gsl_rng.h>

Model::Model(const unsigned int &pop_size, ParamNS::Param *param_set) : n_total(pop_size), param(param_set) {
    n_hour = Helper::DEFAULT_RUNTIME;
    Init_Model();
}

Model::Model(const unsigned int &pop_size, const unsigned int &total_days, ParamNS::Param *param_set) : n_total(pop_size), n_hour(total_days*24), param(param_set) {
    Init_Model();
}


Model::Model(Trial *trial, ParamNS::Param *param_set) : trial(trial), param(param_set){
    n_total = trial->Get_Pop_Size();
    n_cured = trial->Get_Cured();
    n_hour = Helper::DEFAULT_RUNTIME;
    Init_Model();
}


Model::~Model() {
//    Helper::DeletePointer(param);
//    Helper::DeletePointer(Trial);
    Helper::DeletePointer(rand_gen);
    Helper::DeleteVector<Person*>(updatable_person_v);
    Helper::DeleteVector<Person*>(person_v);
}

void Model::Init_Model() {
    rand_gen = new RandomGenerator(this);
    timer = 0;
    // draw initial parasite count
    // draw initial absorbed drug concentration
    person_v.reserve(n_total);
    for (unsigned int i=0; i<n_total; i++){
        person_v.emplace_back(new Person(this));
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

const RandomGenerator *Model::Get_RandomGenerator() const {
    if (rand_gen)
        return  rand_gen;
    else
        throw std::runtime_error("Null pointer to random generator");
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

const double Model::Calculate_Negative_Log_Likelihood() const {
    unsigned sim_cured = Get_Cure_Number();
    double prob = (double)sim_cured / (double)n_total;
//    return rand_gen->PDF_Binomial(n_cured, n_total, prob)
    return -(log( rand_gen->PDF_Binomial(n_cured, n_total, prob) + 1e-8));;
}

