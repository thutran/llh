//
// Created by thu on 5/10/18.
//

#ifndef LIKELIHOOD_DRUG_H
#define LIKELIHOOD_DRUG_H


class Drug {
public:
    explicit Drug(const double &elimination_halflife_in_hour);
    Drug(const double& initial_concentration, const double& elimination_halflife_in_hour);
    Drug(const double& initial_concentration, const double& elimination_halflife_in_hour,
         const double& pmax, const double& ec50, const unsigned short& slope);
    ~Drug();

    void Update_Concentration(const bool &new_dose, const unsigned &time);
    void Init_Drug();
    const double Get_Current_Concentration() const ;
    const double Get_Halflife() const ;
    const double Get_Pmax() const ;
    const double Get_Ec50() const ;
    const unsigned short Get_Slope() const ;

private:
    double concentration;
    double init_concentration;
    const double init_absorption;
    double halflife;
    double ln2_over_halflife;
    double pmax;
    double ec50;
    unsigned short slope;


};


#endif //LIKELIHOOD_DRUG_H
