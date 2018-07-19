//
// Created by thu on 7/18/18.
//

#ifndef LIKELIHOOD_TESTRANDOMGENERATORSINGLETON_H
#define LIKELIHOOD_TESTRANDOMGENERATORSINGLETON_H

#include <iostream>
#include "RandomGeneratorSingleton.h"
#include "RandomGenerator.h"

void Test_Random_Generator_Singleton(){
    std::cout <<  "Test RandomGeneratorSingleton ---\n";
    RandomGeneratorSingleton *rgs = RandomGeneratorSingleton::Get_RandomGeneratorSingleton();
    RandomGeneratorSingleton *rgs2 = RandomGeneratorSingleton::Get_RandomGeneratorSingleton(21);
    RandomGeneratorSingleton *rgs3 = RandomGeneratorSingleton::Get_RandomGeneratorSingleton(212);
    std::cout << "Calling Get_RandomGeneratorSingleton should returns only 1 object: " <<
              rgs << " " << rgs2 << " " << rgs3 << std::endl;
    std::cout << "Rand uniform 0,100: " << rgs->Rand_Uniform(0,100) << std::endl;
    std::cout << "Rand normal 10,2: " << rgs->Rand_Normal(10, 2) << std::endl;
    std::cout << "PDF normal 4,10,2: " << rgs->PDF_Normal(4, 10, 2) << std::endl;

    std::cout << "Rand Multinomial 2 draws out of 50, prob 0.1, 0.9: ";
    unsigned int multi_draw[2] = {0,0};
    double prob[2] = {0.1, 0.9};
    rgs->Rand_Multinomial(2, 50, prob, multi_draw);
    printf("%3d\t%3d\t", multi_draw[0], multi_draw[1]);
    std::cout << std::endl;

    std::cout <<  "\nSame operations with normal RandomGenerator\n";
    RandomGenerator *rng = new RandomGenerator();
    std::cout << "Rand uniform 0,100: " << rng->Rand_Uniform(0,100) << std::endl;
    std::cout << "Rand normal 10,2: " << rng->Rand_Normal(10, 2) << std::endl;
    std::cout << "PDF normal 4,10,2: " << rng->PDF_Normal(4, 10, 2) << std::endl;

    std::cout << "Rand Multinomial 2 draws out of 50, prob 0.1, 0.9: ";
    multi_draw[0] = 0; multi_draw[1] = 0;
    rng->Rand_Multinomial(2, 50, prob, multi_draw);
    printf("%3d\t%3d\t", multi_draw[0], multi_draw[1]);
    std::cout << std::endl;

    std::cout <<  "\nPseudo uniform\n";
    rgs->Init_Rand_Uniform_Pseudo(1.0, 2.0, 0.7);
    std::cout << "1.0 to 2.0 by 0.7: " << rgs->Rand_Uniform_Pseudo() << std::endl;
    std::cout << "1.0 to 2.0 by 0.7: " << rgs->Rand_Uniform_Pseudo() << std::endl;
    std::cout << "1.0 to 2.0 by 0.7: " << rgs->Rand_Uniform_Pseudo() << std::endl;
    rgs->Init_Rand_Uniform_Pseudo(8.5, 9.2, 0.5);
    std::cout << "8.5 to 9.2 by 0.5: " << rgs->Rand_Uniform_Pseudo() << std::endl;
    std::cout << "8.5 to 9.2 by 0.5: " << rgs->Rand_Uniform_Pseudo() << std::endl;
    std::cout << "8.5 to 9.2 by 0.5: " << rgs->Rand_Uniform_Pseudo() << std::endl;
    std::cout << "8.5 to 9.2 by 0.5: " << rgs->Rand_Uniform_Pseudo() << std::endl;


}

#endif //LIKELIHOOD_TESTRANDOMGENERATORSINGLETON_H
