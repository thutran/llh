
#include <iostream>
#include <cmath>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_vector.h>
#include <RandomGeneratorSingleton.h>

#include "StopWatch.h"
#include "cxxopts.hpp"
#ifdef FF_ON
#include "mpir.h"
#include "mpirbased/primeswing.h"
#include "mpirbased/binomial.h"
#endif

#include "ConfigCmake.h"
#include "Helper.h"
#include "Model.h"
#include "Param.h"
#include "Drug.h"
#include "Trial.h"
#include "RandomGenerator.h"

Model *m;
std::vector<Trial*> trial_v;
RandomGeneratorSingleton *rgs;

unsigned model_cured = 0;

double my_f(const gsl_vector * x, void * prm){
    extern Model *m;
    extern std::vector<Trial*> trial_v;

    double sum_nll = 0.0;
    auto *param = (ParamNS::Param*) prm;

    param->Absorb_Gsl_Vector(x);

    if (param->Is_Good()){
        for (auto trial : trial_v){
            if (m != nullptr) {
                m->Reset_Model(trial, param);
//                Helper::DeletePointer(m);
            } else {
                m = new Model(trial, param); // TODO reuse population
            }
//            m = new Model(trial, param, random_generator); // TODO reuse population
            m->Run();
            sum_nll += m->Calculate_Negative_Log_Likelihood();
//            model_cured = m->Get_Cure_Number();
//            std::cout << m->Get_Cure_Number() << std::endl;
        }
    } else
        sum_nll += 1000;
    return sum_nll;
}

/*void test_model(){
    std::vector<Drug*> test_therapy;
    test_therapy.emplace_back(new Drug(4.8)); // artemether
//    test_therapy.emplace_back(new Drug(108.0)); // lumefantrine
    test_therapy.emplace_back(new Drug(0.1)); // dummy
//    Trial *test_trial = new Trial(20, 18, test_therapy, std::vector<unsigned int>({0,8,24,48}));
//    Trial *test_trial = new Trial(44,24, test_therapy, std::vector<unsigned int>({0,8,24,48})); //dummy
    Trial *test_trial = new Trial(1,0, test_therapy, std::vector<unsigned int>({0,24,48,72,96})); //dummy
    ParamNS::Param *test_param = new ParamNS::Param(2);
//    test_param->Replace_Param(std::vector<unsigned short>({(unsigned short)ParamNS::Non_Pmax_Param_Enum::LOG10_PC_MAX,
//                                                           (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA,
//                                                           (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 0,
//                                                           (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1 }),
//                              std::vector<double>({9, 0.2, 0.5, 0.0}));
    Model *test_model = new Model(test_trial, test_param);
    for (unsigned t=0; t<100; ++t){
        std::cout << ((Person*)test_model->person_v.front())->drug_v[0]->init_concentration << "_" << ((Person*)test_model->person_v.front())->drug_v[0]->Get_Current_Concentration() << " ";
        std::cout << ((Person*)test_model->person_v.front())->drug_v[1]->init_concentration << "_" << ((Person*)test_model->person_v.front())->drug_v[1]->Get_Current_Concentration() << std::endl;
        test_model->Tick();
    }
//    test_model->Run();
//    std::cout << "model cured: " << test_model->Get_Cure_Number() << std::endl;
}*/

#ifdef FF_ON
void test_fast_factorial(){
    mpz_t mpir_x;

    mpz_init(mpir_x);
//    PrimeSwing::Factorial(mpir_x, 28);
    Binomial::PrimeBinomial(mpir_x, 5, 3);

    unsigned long int x = mpz_get_ui(mpir_x);
    std::cout << x << std::endl;

    mpz_clear(mpir_x);
}
#endif

#ifdef FF_ON
double Binomial_PDF(const double &p, const unsigned &n, const unsigned &k){
    mpir_ui m_n=n, m_k=k, m_nk=n-k;
    mpf_t m_p, m_pk, m_1pnk; // MPIR float var
    mpz_t m_bin_coef; // MPIR int
    mpf_inits(m_p, m_pk, m_1pnk, NULL); // initialize var to be used with MPIR
    mpz_inits(m_bin_coef, NULL);
    mpf_set_d(m_p, p);

    Binomial::PrimeBinomial(m_bin_coef, n, k); // calculate binomial coefficient
    mpf_pow_ui(m_pk, m_p, m_k);
    mpf_ui_sub(m_p, 1.0, m_p);
    mpf_pow_ui(m_1pnk, m_p, m_nk);

//    printf("m_bin_coeff: "); mpz_out_str(stdout, 10, m_bin_coef);
//    printf("\nm_pk: "); mpf_out_str(stdout, 10, 5, m_pk);
//    printf("\nm_1pnk: "); mpf_out_str(stdout, 10, 5, m_1pnk);
//    printf("\n");

    mpf_mul(m_pk, m_pk, m_1pnk);
    mpf_mul_ui(m_pk, m_pk, mpz_get_ui(m_bin_coef));
    printf("Bin_PDF: "); mpf_out_str(stdout, 10, 8, m_pk);
    printf("\n");

    mpf_clears(m_p, m_pk, m_1pnk, NULL); // release MPIR var
    mpz_clears(m_bin_coef, NULL);
}
#endif

int main(int argc, char* argv[]) {
    StopWatch sw;
    sw.Restart();

    // parsing arguments using cxxopts
/*
    try {
        cxxopts::Options options(argv[0], "=== PKPD parameters estimator using GSL minimizer ===");
        options
                .positional_help("[optional args]")
                .show_positional_help();

        options.add_options()
                ("c,config", "Config file",
                 cxxopts::value<std::string>()->default_value("config.cfg")->implicit_value("config.cfg"), "INPUT_FILE")
                ("o,output", "Output file",
                 cxxopts::value<std::string>()->default_value("llhDemo.out")->implicit_value("llhDemo.out"), "OUTPUT_FILE")
                ("help", "Print this help")
//                ("positional",
//                 "Positional arguments: these are the arguments that are entered "
//                 "without an option", cxxopts::value<std::vector<std::string>>())
                 ;

        options.add_options("Parameters to be estimated and their initial values")
                ("log10pcmax", "Log10 of the max parasite count that can be found in a person",
                 cxxopts::value<double>()->implicit_value(std::to_string(ParamNS::DEFAULT_LOG10_PARASITE_COUNT_MAX)), "LOG10PCMAX")
                ("sigma", "Variability of drug absorption",
                 cxxopts::value<double>()->implicit_value(std::to_string(ParamNS::DEFAULT_DRUG_SIGMA)), "SIGMA")
                ("pmax", "Max fraction of the parasite population to be killed by the drug at full concentration and highest drug absorption, use one 'pmax' option for each drug in the combination act_al",
                 cxxopts::value<std::vector<double>>()->implicit_value(std::to_string(ParamNS::DEFAULT_PMAX)), "PMAX")
                ;

        options.add_options("GSL minimizer")
                ("i,iter", "Max number of iterations in the optimizer",
                 cxxopts::value<int>()->default_value("100")->implicit_value("100"), "max_iter")
                ("t,tol", "Tolerance for the minimizer; this will be used to test the characteristic size and determine stopping point",
                 cxxopts::value<double>()->default_value("1e-3")->implicit_value("1e-3"), "tolerance")
                ;

        options.parse_positional({"log10pcmax", "sigma", "pmax"});

        auto result = options.parse(argc, argv);

        if (result.count("help")){
            std::cout << options.help({"",
                                       "Parameters to be estimated and their initial values",
                                       "GSL minimizer"})
                      << std::endl;
            exit(0);
        }
//        if (result.count("b"))
//        {
//            std::cout << "Saw option ‘b’" << result.count("a") << std::endl;
//        }
        if (result.count("config")){
            std::cout << "Config = " << result["config"].as<std::string>()
                      << std::endl;
        }
        if (result.count("output")){
            std::cout << "Output = " << result["output"].as<std::string>()
                      << std::endl;
        }
//        if (result.count("positional")){
//            std::cout << "Positional = {";
//            auto& v = result["positional"].as<std::vector<std::string>>();
//            for (const auto& s : v) {
//                std::cout << s << ", ";
//            }
//            std::cout << "}" << std::endl;
//        }
        if (result.count("pmax")){
            std::cout << "Pmax = {";
            auto &v = result["pmax"].as<std::vector<double >>();
            for (auto &i :v)
                std::cout << i << ", ";
            std::cout << "}" << std::endl;
        }

        if (result.count("iter")){
            std::cout << "max iteration = " << result["iter"].as<int>() << std::endl;
        }
        if (result.count("tol")){
            std::cout << "tolerance = " << result["tol"].as<double>() << std::endl;
        }

        std::cout << "Arguments remain = " << argc << std::endl;

    } catch (const cxxopts::OptionException& e)
    {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
*/





    // setup
    extern RandomGeneratorSingleton *rgs;
    rgs = RandomGeneratorSingleton::Get_RandomGeneratorSingleton(1111);


    extern std::vector<Trial*> trial_v;
    std::vector<Drug*> act_al;
    act_al.emplace_back(new Drug(4.2)); // artemether
    act_al.emplace_back(new Drug(108.0)); // lumefantrine
    std::vector<Drug*> mono_am;
    mono_am.emplace_back(new Drug(4.2));
    mono_am.emplace_back(new Drug(0.0)); // dummy
    std::vector<Drug*> mono_lm;
    mono_lm.emplace_back(new Drug(0.0)); // dummy
    mono_lm.emplace_back(new Drug(108.0));

    // artemether monotherapy trials
    trial_v.push_back(new Trial(20, 9, mono_am, std::vector<unsigned int>({0,8,24,48})));
    trial_v.push_back(new Trial(38, 28, mono_am, std::vector<unsigned int>({0,12,24,36,48,60,84,96,108})));
    trial_v.push_back(new Trial(49, 48, mono_am, std::vector<unsigned int>({0,12,24,36,48,60,84,96,108,120,132,144,156,168})));
    trial_v.push_back(new Trial(30, 29, mono_am, std::vector<unsigned int>({0,6,24,48,72,96})));
    trial_v.push_back(new Trial(50, 44, mono_am, std::vector<unsigned int>({0,24,48,72,96})));
    trial_v.push_back(new Trial(44, 24, mono_am, std::vector<unsigned int>({0,8,24,48})));
    // lumefantrine monotherapy trials
    trial_v.push_back(new Trial(20, 15, mono_lm, std::vector<unsigned int>({0,8,24,48})));
    trial_v.push_back(new Trial(51, 47, mono_lm, std::vector<unsigned int>({0,8,24,48})));
    trial_v.push_back(new Trial(49, 47, mono_lm, std::vector<unsigned int>({0,8,24,48})));
    trial_v.push_back(new Trial(49, 45, mono_lm, std::vector<unsigned int>({0,8,24,48})));
//    // artemether-lumefantrine combination trials
    trial_v.emplace_back(new Trial(20, 18, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(50, 50, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(78, 62, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(81, 62, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(51, 50, act_al, std::vector<unsigned int>({0,8,24,48})));
    trial_v.emplace_back(new Trial(102, 85, act_al, std::vector<unsigned int>({0,8,24,48})));

    ParamNS::Param *param = new ParamNS::Param(2); // default param set with 2 drugs

    // print csv format
    /*extern Model *m;
    std::cout << "pmax_AM,pmax_LM,sum_negll" << std::endl;
    double pm_am=0.6029665, pm_lm=0.1093061, step=0.005;
    while (pm_am == 0.6029665){
        while (pm_lm == 0.1093061){
            param->Replace_Param(std::vector<unsigned short>({(unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 0,
                                                              (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1}),
                                 std::vector<double >({pm_am, pm_lm}));
            double sum_nll = 0.0;
            for (auto trial : trial_v){
                if (m != nullptr)
                    m->Reset_Model(trial, param);
                else
                    m = new Model(trial, param); // TODO reuse population
                m->Run();
                sum_nll += m->Calculate_Negative_Log_Likelihood();
            }
            std::cout << pm_am << "," << pm_lm << "," << sum_nll << std::endl ;
            pm_lm += 0.01; //step;
        }
        pm_lm=0.09;
        pm_am += step;
    }*/


    // gsl minimizer
//    param->Replace_Param(std::vector<unsigned short>({(unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1}),
//                         std::vector<double >({0.11}));

    param->Set_Search_I(std::vector<unsigned short>({ (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 0,
                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1 }));//,
//                                                      (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIGMA }));
    unsigned search_dim = 2;
    std::vector<double > search_init_values({0.5, 0.5});
    std::vector<double > search_step_size({0.2391, 0.2391});

//    param->Set_Search_I(std::vector<unsigned short>({ (unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 0}));
//    unsigned search_dim = 1;
//    std::vector<double > search_init_values({0.5});
//    std::vector<double > search_step_size({0.09231});

    double i_am=0.0, i_lm=0.0, step=0.01;
    std::cout << "step_size,init_AM,init_LM,iteration,pmax_AM,pmax_LM,sum_negll\n" ;
//    std::cout << "step_size,init_AM,iteration,pmax_AM,pmax_LM,sum_negll\n" ;
    while (i_am < 1.01){
        search_init_values[0] = i_am;
        i_lm = 0.00;
        while (i_lm < 1.01){
            search_init_values[1] = i_lm;
//            param->Replace_Param(std::vector<unsigned short>({(unsigned short)ParamNS::Non_Pmax_Param_Enum::SIZE + 1}),
//                                 std::vector<double >({i_lm}));


            int iter = 0;
            int status;

            const gsl_multimin_fminimizer_type *T = gsl_multimin_fminimizer_nmsimplex2; // type
            gsl_multimin_fminimizer *s = nullptr;
            gsl_multimin_function my_func;
            gsl_vector *x, *step_size;

            my_func.n = search_dim;
            my_func.f = &my_f;
            my_func.params = param;

            // Starting point, x = (5,7)
            x = gsl_vector_alloc (search_dim); // candidate params
            for (unsigned i=0; i<search_dim; ++i){
                gsl_vector_set (x, i, search_init_values[i]); // pmax LM
            }

            step_size = gsl_vector_alloc (search_dim); // define stepsize for each dimension
            for (unsigned i=0; i<search_dim; ++i){
                gsl_vector_set (step_size, i, search_step_size[i]);
            }

            s = gsl_multimin_fminimizer_alloc (T, search_dim); // allocation
            gsl_multimin_fminimizer_set (s, &my_func, x, step_size); // "f" is then evaluated 1+search_dim times inside set method of the minimizer type class

            do
            {
                iter++;
                status = gsl_multimin_fminimizer_iterate (s);

                if (status)
                    break;

                status = gsl_multimin_test_size (s->size, 1e-8);

                if (status == GSL_SUCCESS){
                    printf("%.5f%c", search_step_size[0], ','); // step size
                    printf("%.5f%c", search_init_values[0], ','); // init AM
                    printf("%.5f%c", search_init_values[1], ','); // init LM
                    printf("%3d%c", iter, ','); // iteration

                    printf("%.10f%c", gsl_vector_get (s->x, 0), ','); // estimated pmax AM
                    printf("%.10f%c", gsl_vector_get (s->x, 1), ','); // estimated pmax LM

                    printf ("%.10f\n", s->fval); // sum neg loglikelihoods
                }

            } while (status == GSL_CONTINUE && iter < 200);

            gsl_multimin_fminimizer_free (s);
            gsl_vector_free (x);
            gsl_vector_free (step_size);


            i_lm += step;
        }
        i_am += step;
    }

//    std::cout << "iteration\t" << "pmaxAM\t" << "pmaxLM\t" << "negll\t" << "simplexSize" << std::endl;
////    std::cout << "init_AM,init_LM,iteration,result_AM,result_LM,negll\n" ;
//
//    int iter = 0;
//    int status;
//
//    const gsl_multimin_fminimizer_type *T = gsl_multimin_fminimizer_nmsimplex2; // type
//    gsl_multimin_fminimizer *s = nullptr;
//    gsl_multimin_function my_func;
//    gsl_vector *x, *step_size;
//
//    my_func.n = search_dim;
//    my_func.f = &my_f;
//    my_func.params = param;
//
//    // Starting point, x = (5,7)
//    x = gsl_vector_alloc (search_dim); // candidate params
//    for (unsigned i=0; i<search_dim; ++i){
//        gsl_vector_set (x, i, search_init_values[i]); // pmax LM
//    }
//
//    step_size = gsl_vector_alloc (search_dim); // define stepsize for each dimension
//    for (unsigned i=0; i<search_dim; ++i){
//        gsl_vector_set (step_size, i, search_step_size[i]);
//    }
//
//    s = gsl_multimin_fminimizer_alloc (T, search_dim); // allocation
//    gsl_multimin_fminimizer_set (s, &my_func, x, step_size); // "f" is then evaluated 1+search_dim times inside set method of the minimizer type class
//
//    do
//    {
//        iter++;
//        status = gsl_multimin_fminimizer_iterate (s);
//
//        if (status)
//            break;
//
//        status = gsl_multimin_test_size (s->size, 1e-8);
//
////            param->Print();
//        if (iter % 10 == 0 || status == GSL_SUCCESS){
////            std::cout << "cured: " << model_cured << std::endl;
//            printf("%5d \t", iter);
//            for (unsigned i=0; i<x->size; ++i){
//                printf("%.10f \t", gsl_vector_get (s->x, i));
//            }
//            printf ("%.8f \t %.10f", s->fval, s->size );
//            if (status == GSL_SUCCESS)
//                printf("%s\n", " *** ");
//            else
//                printf("\n");
//        }
//
//
//    } while (status == GSL_CONTINUE && iter < 200);
//
//    gsl_multimin_fminimizer_free (s);
//    gsl_vector_free (x);
//    gsl_vector_free (step_size);
//
//    std::cout << "Elapsed time (sec): " << sw.ElapsedSec() << std::endl;


//    test_model();
    #ifdef FF_ON
    StopWatch ff_sw;
    ff_sw.Restart();
//    test_fast_factorial();
    Binomial_PDF(0.5, 30, 10);
    std::cout << "Elapsed time (microsec): " << ff_sw.ElapsedUs() << std::endl;

    RandomGenerator *rng = new RandomGenerator();
    ff_sw.Restart();
    std::cout << rng->PDF_Binomial(10, 30, 0.5) << std::endl;
    std::cout << "Elapsed time (microsec): " << ff_sw.ElapsedUs() << std::endl;
    #endif

    return 0;
}
