//
// Created by thu on 5/10/18.
//

#include "Trial.h"

Trial::Trial(const unsigned int &size, const unsigned int &cured, const std::vector<Drug *> &drugs,
             const std::vector<unsigned int> &drug_intake_hours) : n_total(size), n_cured(cured), drug_v(drugs), new_dose_hour_v(drug_intake_hours) {

}

Trial::~Trial() {

}

const std::vector<Drug *> Trial::Get_Drug_V() const {
    return drug_v;
}

const std::vector<unsigned int> Trial::Get_New_Dose_Hour_V() const {
    return new_dose_hour_v;
}

const unsigned int Trial::Get_Pop_Size() const {
    return n_total;
}

const unsigned int Trial::Get_Cured() const {
    return n_cured;
}
