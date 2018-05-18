//
// Created by thu on 5/10/18.
//

#ifndef LIKELIHOOD_TRIAL_H
#define LIKELIHOOD_TRIAL_H

#include <vector>

class Drug;

class Trial {
public:
    unsigned int n_total;
    unsigned int n_cured;
    std::vector<Drug*> drug_v;
    std::vector<unsigned int> new_dose_hour_v;

    Trial(const unsigned int &size, const unsigned int &cured,
          const std::vector<Drug *> &drugs, const std::vector<unsigned int> &drug_intake_hours);
    ~Trial();

    const std::vector<Drug*> Get_Drug_V() const;
    const std::vector<unsigned int> Get_New_Dose_Hour_V() const;
    const unsigned int Get_Pop_Size() const ;
    const unsigned int Get_Cured() const ;
};


#endif //LIKELIHOOD_TRIAL_H
