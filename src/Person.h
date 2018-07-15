//
// Created by thu on 5/9/18.
//

#ifndef LIKELIHOOD_PERSON_H
#define LIKELIHOOD_PERSON_H

#include <vector>

class Model;
class ParasiteClone;
class Drug;

class Person {
public:
    Model *model;
    ParasiteClone *parsite_clone;
    std::vector<unsigned int> new_dose_hour_v;
    std::vector<Drug*> drug_v;
//    std::vector<double> pmax_v;
    bool updatable = true;


    explicit Person(Model *model);
    Person(Model *model, const double &total_parasite_count, const unsigned short &mean_age,
           const unsigned short &parasite_count_sd);
    void Reset_Person(Model *model, const double &total_parasite_count, const unsigned short &mean_age,
                      const unsigned short &parasite_count_sd);
    ~Person();
    void Init_Person();
    void Init_Parasite_Clone();
    void Init_Parasite_Clone(const double &total_parasite_count, const unsigned short &mean_age,
                             const unsigned short &parasite_count_sd);
    void Reset_Parasite_Clone(const double &total_parasite_count, const unsigned short &mean_age,
                              const unsigned short &parasite_count_sd);
    void Init_Drug_V();
    void Add_Drug(const double &init_concentration, const double &half_life,
                  const double &pmax, const double &ec50, const unsigned short &slope);
    void Update_Person();
    const bool Is_Cured() const ;
    const Model* Get_Model() const ;
    const std::vector<Drug*> Get_Drug_V() const ;
    const bool Is_Updatable() const ;
};


#endif //LIKELIHOOD_PERSON_H
