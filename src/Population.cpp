//
// Created by thu on 7/11/18.
//

#include <cmath>
#include "Population.h"
#include "Person.h"
#include "RandomGenerator.h"
#include "Helper.h"
#include "RandomGeneratorSingleton.h"

Population::Population(const unsigned& pop_size, Model *m, ParamNS::Param *param_set, RandomGenerator* random_gen) : model(m),
                                                                                                            param(param_set),
                                                                                                            rand_gen(random_gen),
                                                                                                            population_size(pop_size) {
    rand_gen_singleton = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
    for (unsigned int i=0; i<pop_size; ++i){
        // draw initial parasite count
        const auto &prm_log10_pc_max = param_set->Get_Non_Pmax_Param((unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX);
        double total = rand_gen_singleton->Rand_Uniform(1,9) * pow(10,
                                                         rand_gen_singleton->Rand_Uniform(ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MIN, prm_log10_pc_max));
//        double total = 1e10;
//    const auto &prm_pa_mean = param->Get_Non_Pmax_Param((unsigned short)ParamNS::Non_Pmax_Param_Enum::PA_MEAN);
//    const auto &prm_pa_sd = param->Get_Non_Pmax_Param((unsigned short)ParamNS::Non_Pmax_Param_Enum::PA_SD);
//    unsigned short mean_age = (unsigned short)rand_gen->Rand_Normal(prm_pa_mean, 5);
//    unsigned short sd_age = (unsigned short)rand_gen->Rand_Normal(prm_pa_sd, 3);
        unsigned short mean_age = 26;//(unsigned short)rand_gen->Rand_Uniform(0, ParamNS::MAX_PARASITE_HOUR - 1);
        unsigned short sd_age = 10;//(unsigned short)rand_gen->Rand_Uniform(0, ParamNS::MAX_PARASITE_HOUR - 1);


        person_v.emplace_back(new Person(m, total, mean_age, sd_age));
        // draw initial absorbed drug concentration
    }
    updatable_person_v = person_v;
}

Population::~Population() {
    Helper::DeleteVector<Person*>(updatable_person_v);
    Helper::DeleteVector<Person*>(person_v);
}

void Population::Reset_Population(Model *m, ParamNS::Param *param_set, RandomGenerator *random_gen) {
    this->model = m;
    this->param = param_set;
    this->rand_gen = random_gen;

}
