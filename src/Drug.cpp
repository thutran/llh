//
// Created by thu on 5/10/18.
//

#include <cmath>
//#include <iostream>
#include "Drug.h"
#include "Helper.h"
#include "Param.h"

Drug::Drug(const double &elimination_halflife_in_hour) : init_concentration(1.0), halflife(elimination_halflife_in_hour) {
    Init_Drug();
}

Drug::Drug(const double &initial_concentration, const double &elimination_halflife_in_hour) : init_concentration(initial_concentration), halflife(elimination_halflife_in_hour) {
    Init_Drug();
}

Drug::Drug(const double &initial_concentration, const double &elimination_halflife_in_hour, const double &pmax,
           const double &ec50, const unsigned short &slope) : init_concentration(initial_concentration), halflife(elimination_halflife_in_hour), pmax(pmax), ec50(ec50), slope(slope) {
    Init_Drug();
}

void Drug::Init_Drug() {
    ln2_over_halflife = Helper::LN2/halflife;
    concentration = 0.0;
}

Drug::~Drug() {

}

const double Drug::Get_Current_Concentration() const {
    return concentration;
}

void Drug::Update_Concentration(const bool &new_dose) {
    /*if (new_dose)
        concentration = concentration*exp(-ln2_over_halflife) + init_concentration;
    else if (concentration > ParamNS::DEFAULT_DRUG_EFFECTIVE_CONC )
        concentration = concentration*exp(-ln2_over_halflife);
    else
        concentration = 0.0;*/
    concentration = concentration*exp(-ln2_over_halflife) + (new_dose ? init_concentration : 0);
}

const double Drug::Get_Halflife() const {
    return halflife;
}

const double Drug::Get_Pmax() const {
    return pmax;
}

const unsigned short Drug::Get_Slope() const {
    return slope;
}

const double Drug::Get_Ec50() const {
    return ec50;
}
