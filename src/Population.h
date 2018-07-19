//
// Created by thu on 7/11/18.
//

#ifndef LIKELIHOOD_POPULATION_H
#define LIKELIHOOD_POPULATION_H

#include <vector>
#include "Param.h"

class RandomGeneratorSingleton;
class Model;
class Person;
class RandomGenerator;

class Population {
public:

    Population(const unsigned& pop_size, Model *m, ParamNS::Param *param_set, RandomGenerator* random_gen);
    void Reset_Population(Model *m, ParamNS::Param *param_set, RandomGenerator* random_gen);
    ~Population();

private:
    Model *model;
    ParamNS::Param *param;
    unsigned population_size;
    std::vector<Person*> person_v;
    std::vector<Person*> updatable_person_v;
    RandomGenerator *rand_gen;
    RandomGeneratorSingleton *rand_gen_singleton;
};


#endif //LIKELIHOOD_POPULATION_H
