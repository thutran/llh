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
Trial* trial;

double my_f(const gsl_vector * x, void * prm){
    extern Model *m;
    extern Trial *trial;
//    extern ParamNS::Param *param;
    ParamNS::Param *tmp = (ParamNS::Param*) prm;

    tmp->Absorb_Gsl_Vector(x);

    // check good set of param, if not, introduce penalty
    if (m){
        if (!tmp->Is_Good())
            return (m->negative_log_likelihood + 10);
        Helper::DeletePointer(m);
    }
//    ParamNS::Param *param = (tmp->Is_Good()) ? tmp : (ParamNS::Param*) prm;

//    m = new Model(trial, param); // TODO reuse population
    m = new Model(trial, tmp); // TODO reuse population
//    std::cout << m->Get_RandomGenerator()->seed << std::endl;
    m->Run();
//    std::cout << m->Get_Cure_Number() << std::endl;
    return m->Calculate_Negative_Log_Likelihood();
}

int main() {
    StopWatch sw;
    sw.Restart();

    extern Trial *trial;

    std::vector<Trial*> trial_v;
    std::vector<Drug*> therapy;
    therapy.emplace_back(new Drug(4.2)); // artemether
    therapy.emplace_back(new Drug(108.0)); // lumefantrine
//    therapy.push_back(new Drug(0.1)); // dummy
//    trial_v.emplace_back(new Trial(100, 3, therapy, std::vector<unsigned int>({0,8,24,48}))); // dummy

    trial_v.emplace_back(new Trial(20, 18, therapy, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(50, 50, therapy, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(78, 62, therapy, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(81, 62, therapy, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(51, 50, therapy, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(102, 85, therapy, std::vector<unsigned int>({0,8,24,48})));
//    trial_v.push_back(new Trial(1000, 1, therapy, std::vector<unsigned int>({0,24,48,72,96})));
//    trial_v.push_back(new Trial(1000, 1, therapy, std::vector<unsigned int>({0,12,24,36,48,60,84,96,108})));

    ParamNS::Param *param = new ParamNS::Param(2); // default param set with 2 drugs
    unsigned search_dim = 3;
    param->Set_Search_I(std::vector<unsigned short>({ (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1,
                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 0,
                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA }));//,
//                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX}));
//    param->Print();
//    param->Replace_Param(std::vector<unsigned short >({(unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX,
//                                                       (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA,
//                                                       (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 0,
//                                                       (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1}),
//                         std::vector<double>({13, 0.0, 0.0, 0.0}));
//    param->Print();

//    Model *m = new Model(trial_v.front(), param);
//    m = new Model(trial_v.front(), param);
//    m->Run();
//    std::cout << "cured: " << m->Get_Cure_Number()
//              << "\nnegative log-likelihood: " << m->Calculate_Negative_Log_Likelihood() << std::endl;

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
    printf("%s \t %s \t %s \t %s \t %s \t %s \t %s \t %s \n",
           "total", "cured", "iteration", "pmaxLM", "pmaxAM", "sigma", "negll", "simplexSize");
    while (!trial_v.empty()){
        trial = trial_v.back();
        trial_v.pop_back();
        unsigned trial_total = trial->Get_Pop_Size();
        unsigned trial_cured = trial->Get_Cured();

//        std::cout << "Trial: cured " << trial->Get_Cured() << " out of " << trial->Get_Pop_Size() << std::endl;

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
        gsl_vector_set (x, 0, 0.4); // pmax LM
        gsl_vector_set (x, 1, 0.7); // pmax AM
        gsl_vector_set (x, 2, 0.1); // sigma
//        gsl_vector_set (x, 3, 13); // log10 pc max
//        gsl_vector_set_all(x, 0.1);

        step_size = gsl_vector_alloc (search_dim); // define stepsize for each dimension
        gsl_vector_set (step_size, 0, 0.001); //
        gsl_vector_set (step_size, 1, 0.001); //
        gsl_vector_set (step_size, 2, 0.1); //
//        gsl_vector_set (step_size, 3, 1.0); //
//        gsl_vector_set_all(step_size, 0.1);

        T = gsl_multimin_fminimizer_nmsimplex2rand; // type
        s = gsl_multimin_fminimizer_alloc (T, search_dim); // allocation

        gsl_multimin_fminimizer_set (s, &my_func, x, step_size); // "f" is then evaluated 1+search_dim times inside set method of the minimizer type class

        do
        {
            iter++;
            status = gsl_multimin_fminimizer_iterate (s);

            if (status)
                break;

            status = gsl_multimin_test_size (s->size, 1e-3);

//            param->Print();
            if (iter % 10 == 0 || status == GSL_SUCCESS){
                printf ("%4d \t %4d \t %5d \t %.5f \t %.5f \t %.5f \t %.5f \t %.5f",
                        trial_total,
                        trial_cured,
                        iter,
                        gsl_vector_get (s->x, 0),
                        gsl_vector_get (s->x, 1),
                        gsl_vector_get (s->x, 2),
//                        gsl_vector_get (s->x, 3),
                        s->fval,
                        s->size );
                if (status == GSL_SUCCESS)
                    printf("%s\n", " *** ");
                else
                    printf("\n");

            }
        } while (status == GSL_CONTINUE && iter < 3000);

        gsl_multimin_fminimizer_free (s);
        gsl_vector_free (x);
        gsl_vector_free (step_size);
    }

    std::cout << "Elapsed time (sec): " << sw.ElapsedSec() << std::endl;

    return 0;
}
