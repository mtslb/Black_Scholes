#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include<algorithm>
#include<cmath>
#include<stdexcept>

// Classe abstraite Option
class Option{
    protected:
        double K_;
        double L_;
        double r_;
        double T_;

    public:
        Option(double K, double L, double r, double T); // constructeur
        virtual ~Option(); // destructeur virtuel
        virtual double payoff(double S) const = 0; // méthode virtuelle pure (=0)
        virtual double boundary_condition_low(double t) const = 0; // méthode virtuelle pure (=0)
        virtual double boundary_condition_high(double t) const = 0; // méthode virtuelle pure (=0)
        double get_strike_price() const; // Getter pour K_
};

// Classe pour Call
class Call : public Option{ // Classe dérivée de Option
    public:
        Call(double K, double L, double r, double T); //Option(K, L, r, T) {}
        double payoff(double S) const override;
        double boundary_condition_low(double t) const override;     //override car dérivée d'une méthode virtuelle pure on réecrit la méthode pour Call
        double boundary_condition_high(double t) const override;    //override car dérivée d'une méthode virtuelle pure on réecrit la méthode pour Call
};

// Classe pour Put
class Put : public Option{  // Classe dérivée de Option
    public:
        Put(double K, double L, double r, double T); //Option(K, L, r, T) {}
        double payoff(double S) const override; 
        double boundary_condition_low(double t) const override;     //override car dérivée d'une méthode virtuelle pure on réecrit la méthode pour Put
        double boundary_condition_high(double t) const override;    //override car dérivée d'une méthode virtuelle pure on réecrit la méthode pour Put
};

#endif // PAYOFF_HPP
