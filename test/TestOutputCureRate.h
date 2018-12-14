//
// Created by thu on 8/17/18.
//

#ifndef LIKELIHOOD_TESTOUTPUTCURERATE_H
#define LIKELIHOOD_TESTOUTPUTCURERATE_H

#include <vector>
#include "RandomGeneratorSingleton.h"
#include "Drug.h"
#include "Trial.h"
#include "Model.h"

void Test_Output_Cure_Rate(){
    std::cout <<  "Test OutputCureRate ---\n";
    RandomGeneratorSingleton *rgs = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
//    RandomGeneratorSingleton *rgs = RandomGeneratorSingleton::Get_RandomGeneratorSingleton(1111);

//    Drug *st0 = new Drug(1.0, /*0.0*/2.41*24, 0.99, 0.6, 20);
//    Drug *st1 = new Drug(1.0, /*0.0*/2.41*24, 0.99, 1.3, 20);
    Drug *st0 = new Drug(1.0, 4.0, 0.999, 0.75, 25);
    Drug *st1 = new Drug(1.0, 4.0, 0.999, 1.2, 25);
    Drug *lg0 = new Drug(1.0, /*0.0*/28.0*24, 0.9, 0.58, 15);
    Drug *lg1 = new Drug(1.0, /*0.0*/28.0*24, 0.9, 1.4, 15);

    std::vector<Drug*> st0_lg0({st0, lg0});
    std::vector<Drug*> st1_lg0({st1, lg0});
    std::vector<Drug*> st0_lg1({st0, lg1});
    std::vector<Drug*> st1_lg1({st1, lg1});

    std::vector<unsigned> admin1({0});
    std::vector<unsigned> admin3({0, 24, 48});

    std::vector<Trial*> trial_v;
    trial_v.emplace_back(new Trial(100, 100, st0_lg0, admin1));
    trial_v.emplace_back(new Trial(100, 100, st0_lg0, admin3));
    trial_v.emplace_back(new Trial(100, 100, st1_lg0, admin1));
    trial_v.emplace_back(new Trial(100, 100, st1_lg0, admin3));
    trial_v.emplace_back(new Trial(100, 100, st0_lg1, admin1));
    trial_v.emplace_back(new Trial(100, 100, st0_lg1, admin3));
    trial_v.emplace_back(new Trial(100, 100, st1_lg1, admin1));
    trial_v.emplace_back(new Trial(100, 100, st1_lg1, admin3));

    ParamNS::Param *param = new ParamNS::Param(2);
    param->Replace_Param(std::vector<unsigned short>({(unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 0,
                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1}),
                         std::vector<double >({0.99, 0.9}));

    Model *m = nullptr;
    /*m = new Model(trial_v.front(), param);
    m->Run();
    printf("%.5f\n", (double)m->Get_Cure_Number()/ (double)m->Get_Total_Pop() ); // cure rate*/

//    Trial *trial;
//    for (unsigned i=0; i<8; ++i){
//        trial = trial_v[i];
    for (auto &trial : trial_v){

        if (m != nullptr){
            m->Reset_Model(trial, param);
        }
        else
            m = new Model(trial, param); // TODO reuse population
        m->Run();

        printf("%.5f\n", (double)m->Get_Cure_Number()/ (double)m->Get_Total_Pop() ); // cure rate

    }

}

#endif //LIKELIHOOD_TESTOUTPUTCURERATE_H
