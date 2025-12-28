#include "edp.hpp"


//Implémentation des méthodes de la classe EDP
EDP::EDP(Option* option, double sigma, double r, double T, double L)
    : option_(option), sigma_(sigma), r_(r), T_(T), L_(L) {} //constructeur

//Getters
Option* EDP::getOption() const {
    return option_;
}
double EDP::getSigma() const {
    return sigma_;
}
double EDP::getR() const {
    return r_;
}
double EDP::getT() const {
    return T_;
}   
double EDP::getL() const {
    return L_;
}


//Implémentation des méthodes de la classe EDP_complete
EDP_complete::EDP_complete(Option* option, double sigma, double r, double T, double L)
    : EDP(option, sigma, r, T, L) {}     //appel du constructeur de la classe mère EDP

//Implémentation des méthodes de la classe EDP_reduite
EDP_reduite::EDP_reduite(Option* option, double sigma, double r, double T, double L)
    : EDP(option, sigma, r, T, L) {}    //appel du constructeur de la classe mère EDP