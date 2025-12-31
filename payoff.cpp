#include "payoff.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>

//Implémentation des méthodes de la classe Option

Option::Option(double K, double L, double r, double T) //Constructeur de la classe Option
    : K_(K), L_(L), r_(r), T_(T) {}

Option::~Option() {} //Destructeur virtuel de la classe Option

double Option::get_strike_price() const {return K_;}



//Implémentation des méthodes de la classe Call

Call::Call(double K, double L, double r, double T) //constructeur de la classe Call
    : Option(K, L, r, T) {} //appel du constructeur de la classe mère Option
double Call::payoff(double S) const { //Implémentation de la méthode payoff pour Call
    if (S > L_) {
        throw std::out_of_range("S is out of range");
    }
    return std::max(S - K_, 0.0);
}
double Call::boundary_condition_low(double t) const { //Implémentation de la condition à la limite S=0 pour Call
    return t*0.0;
}
double Call::boundary_condition_high(double t) const { //Implémentation de la condition à la limite S=L pour Call
    return L_ - K_ * std::exp(-r_ * (t -T_));
}



//Implémentation des méthodes de la classe Put

Put::Put(double K, double L, double r, double T) //constructeur de la classe Put
    : Option(K, L, r, T) {} //appel du constructeur de la classe mère Option
double Put::payoff(double S) const { //Implémentation de la méthode payoff pour Put
    if (S > L_) {
        throw std::out_of_range("S is out of range");
    }
    return std::max(K_ - S, 0.0);  
}
double Put::boundary_condition_low(double t) const { //Implémentation de la condition à la limite S=0 pour Put
    return K_ * std::exp(-r_ *(T_ - t)); 
}
double Put::boundary_condition_high(double t) const { //Implémentation de la condition à la limite S=L pour Put
    return t*0.0;
}