//
// Created by thu on 5/9/18.
//

#ifndef LIKELIHOOD_PARASITECLONE_H
#define LIKELIHOOD_PARASITECLONE_H

#include <vector>

class Person;
class Drug;

class ParasiteClone {
public:
    Person *person;
    bool dirty = true;
    double total_count;
    std::vector<double> parasite_count_bins; // parasite count at each age-hour
    std::vector<double> max_afftected_fraction_bins; // age-dependent maximum fraction that can be affected by drug

    explicit ParasiteClone(Person *person);
    ParasiteClone(Person *person,
                  const double &total_parasite_count,
                  const unsigned short &mean_age, const unsigned short &std_deviation);
    ~ParasiteClone();
    void Distribute_To_Bins(const double &total_parasite_count,
                            const unsigned short &mean_age, const unsigned short &std_deviation);
    void Init_Max_Affected_Fraction_Bins();
    const double Get_Total_Parasite_Count()  ;
    void Killed_By_Drug(const std::vector<Drug*>& drug_v);
    void Update_ParasiteClone();
};


#endif //LIKELIHOOD_PARASITECLONE_H
