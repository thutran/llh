//
// Created by thu on 5/8/18.
//

#ifndef LIKELIHOOD_MODEL_H
#define LIKELIHOOD_MODEL_H

#include <vector>
#include "Param.h"

class RandomGeneratorSingleton;
class RandomGenerator;
class Trial;
class Person;

class Model {
public:
    Model(const unsigned int &pop_size, ParamNS::Param *param_set);
    Model(const unsigned int &pop_size, const unsigned int &total_days, ParamNS::Param *param_set);
    Model(Trial *trial, ParamNS::Param *param_set);
    ~Model();

    void Reset_Model(Trial *trial, ParamNS::Param *param_set);
    void Reset_Person_V();
    void Init_Model();
    void Init_Person_V();
    void Run();
    void Tick();
    unsigned int Get_Total_Pop() const;
    const unsigned int Get_Cure_Number() const ;
    const ParamNS::Param* Get_Param_Set() const ;
    const Trial* Get_Trial() const ;
    const unsigned int Get_Timer() const ;
    RandomGeneratorSingleton* Get_RandomGeneratorSingleton() const;
    const double Calculate_Negative_Log_Likelihood() ;
    const double Get_Negative_Log_Likelihood() const ;

private:
    unsigned scale_up_n_total = 2;
    unsigned n_total; // total population
    unsigned n_hour; // duration of simulation in hour
    ParamNS::Param *param; // set of parameters to run the PKPD model
    Trial *trial;
    RandomGeneratorSingleton *rand_gen_singleton;
    std::vector<Person*> person_v;
    std::vector<Person*> updatable_person_v;
    double negative_log_likelihood; // this should be the sum of nll with different trials
    unsigned int timer;
};


#endif //LIKELIHOOD_MODEL_H
