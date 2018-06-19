#include <iostream>
#include <cmath>
#include <gsl/gsl_multimin.h>
#include <Person.h>
#include <StopWatch.h>
#include "ConfigCmake.h"

#include "Helper.h"
#include "Model.h"
#include "Param.h"
#include "RandomGenerator.h"
#include "ParasiteClone.h"
#include "Drug.h"
#include "Trial.h"

Model *m;
//std::vector<Trial*> trial_v;
Trial* trial;
ParamNS::Param *param;

double my_f(const gsl_vector * x, void * prm){
    extern Model *m;
    extern Trial *trial;
    extern ParamNS::Param *param;

//    ParamNS::Param *param = static_cast<ParamNS::Param *>(prm);
    param->Absorb_Gsl_Vector(x);

    // check good set of param, if not, introduce penalty
    if (!param->Is_Good())
        return (m->negative_log_likelihood + 1000);

    if (m)
        Helper::DeletePointer(m);

    m = new Model(trial, param); // TODO reuse population
    m->Run();
    return m->Calculate_Negative_Log_Likelihood();
}

int main() {
//    extern Model *m;
//    extern std::vector<Trial*> trial_v;
    extern Trial *trial;
    extern ParamNS::Param *param;

    StopWatch sw;
    sw.Restart();

    std::vector<Trial*> trial_v;
    std::vector<Drug*> act_al;
    act_al.emplace_back(new Drug(4.2)); // artemether
    act_al.emplace_back(new Drug(108.0)); // lumefantrine
//    act_al.push_back(new Drug(0.1)); // dummy
//    std::vector<Trial*> trial_v;
//    trial_v.emplace_back(new Trial(5, 3, act_al, std::vector<unsigned int>({0,8,24,48}))); // dummy

    trial_v.emplace_back(new Trial(20, 18, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(50, 50, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(78, 62, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(81, 62, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(51, 50, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(102, 85, act_al, std::vector<unsigned int>({0,8,24,48})));
//    trial_v.push_back(new Trial(1000, 1, act_al, std::vector<unsigned int>({0,24,48,72,96})));
//    trial_v.push_back(new Trial(1000, 1, act_al, std::vector<unsigned int>({0,12,24,36,48,60,84,96,108})));

//    ParamNS::Param *param = new ParamNS::Param(std::vector<double>({0.9,0.1}), 0.5); // params with initial values
//    param = new ParamNS::Param(std::vector<double>({0.5,0.5}), 0.5); // params with initial values
    param = new ParamNS::Param(2); // params with initial values and search indices
    unsigned search_dim = 4;
    param->Set_Search_I(std::vector<unsigned short>({ (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA + 2,
                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA + 1,
                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA,
                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX}));
//    param->Print();
//    std::cout << "replace!" << std::endl;
//    param->Replace_Param(std::vector<unsigned short >({(unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX,
//                                                       (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA,
//                                                       (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA + 1,
//                                                       (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA + 2}),
//                         std::vector<double>({14, 0.0, 0.0, 0.0}));
    param->Print();
//    Model *m = new Model(trial_v.front(), param);
//    m = new Model(trial_v.front(), param);
//    m->Run();
//    std::cout << "cured: " << m->Get_Cure_Number()
//              << "\nnegative log-likelihood: " << m->Calculate_Negative_Log_Likelihood();

    // test Absorb_Gsl_Vector in Param
//    gsl_vector *x = gsl_vector_alloc (4); // candidate params
//    gsl_vector_set (x, 0, 0.5); // pmax LM
//    gsl_vector_set (x, 1, 0.9); // pmax AM
//    gsl_vector_set (x, 2, 0.1); // sigma
//    gsl_vector_set (x, 3, 12); // log10 parasite count max
//    std::cout << "absorb!" << std::endl;
//    param->Absorb_Gsl_Vector(x);
//    param->Print();


//    std::cout << (int)ParamNS::Non_Pmax_Param_Enum::SIZE << std::endl;

    ///////////////////// GSL minimizer
    while (!trial_v.empty()){
        trial = trial_v.back();
        trial_v.pop_back();

        std::cout << "Trial: cured " << trial->Get_Cured() << " out of " << trial->Get_Pop_Size() << std::endl;

        int iter = 0;
        int status;

        const gsl_multimin_fminimizer_type *T;
        gsl_multimin_fminimizer *s;


        gsl_vector *x, *step_size;
        gsl_multimin_function my_func;

        my_func.n = search_dim;
        my_func.f = &my_f;
        my_func.params = param;

        // Starting point, x = (5,7)
        x = gsl_vector_alloc (search_dim); // candidate params
        gsl_vector_set (x, 0, 0.5); // pmax LM
        gsl_vector_set (x, 1, 0.8); // pmax AM
        gsl_vector_set (x, 2, 0.1); // sigma
        gsl_vector_set (x, 3, 9); // log10 pc max

        step_size = gsl_vector_alloc (search_dim); // define stepsize for each dimension
        gsl_vector_set (step_size, 0, 0.001); //
        gsl_vector_set (step_size, 1, 0.001); //
        gsl_vector_set (step_size, 2, 0.1); //
        gsl_vector_set (step_size, 3, 0.5); //

        T = gsl_multimin_fminimizer_nmsimplex; // type
        s = gsl_multimin_fminimizer_alloc (T, search_dim); // allocation

        gsl_multimin_fminimizer_set (s, &my_func, x, step_size);

        do
        {
            iter++;
            status = gsl_multimin_fminimizer_iterate (s);

            if (status)
                break;

            status = gsl_multimin_test_size (s->size, 5e-4);

            if (status == GSL_SUCCESS){
                printf ("Minimum found at:\n");
                printf ("%5d %.5f %.5f %.5f %.5f %10.5f\n", iter,
                        gsl_vector_get (s->x, 0),
                        gsl_vector_get (s->x, 1),
                        gsl_vector_get (s->x, 2),
                        gsl_vector_get (s->x, 3),
                        s->fval);
                std::cout << "size: " << s->size << std::endl;
            }
        }
        while (status == GSL_CONTINUE && iter < 3000);

        gsl_multimin_fminimizer_free (s);
        gsl_vector_free (x);
        gsl_vector_free (step_size);
    }

    std::cout << "Elapsed time (sec): " << sw.ElapsedSec() << std::endl;

    return 0;
}
