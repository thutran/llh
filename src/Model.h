//
// Created by thu on 5/8/18.
//

#ifndef LIKELIHOOD_MODEL_H
#define LIKELIHOOD_MODEL_H

#include <vector>
#include "Param.h"

class RandomGenerator;
class Trial;
class Person;

class Model {
public:
    unsigned int n_total; // total population
    unsigned int n_cured; // number of cured cases at day 28
    unsigned int n_hour; // duration of simulation in hour
    ParamNS::Param *param; // set of parameters to run the PKPD model
    Trial *trial;
    RandomGenerator *rand_gen;
    std::vector<Person*> person_v;
    std::vector<Person*> updatable_person_v;
    double negative_log_likelihood;
    unsigned int timer;

    Model(const unsigned int &pop_size, ParamNS::Param *param_set);
    Model(const unsigned int &pop_size, const unsigned int &total_days, ParamNS::Param *param_set);
    Model(Trial *trial, ParamNS::Param *param_set);
    ~Model();

    void Init_Model();
    void Run();
    void Tick();
    const unsigned int Get_Cure_Number() const ;
    const RandomGenerator* Get_RandomGenerator() const ;
    const ParamNS::Param* Get_Param_Set() const ;
    const Trial* Get_Trial() const ;
    const unsigned int Get_Timer() const ;
    const double Calculate_Negative_Log_Likelihood() ;
    const double Get_Negative_Log_Likelihood() const ;
};


#endif //LIKELIHOOD_MODEL_H
