#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include<algorithm>
#include<stdexcept>

class option{
    protected:
    double K_;
    double L_;
    double r_;

    public:
    option(double K, double L, double r): K_(K), L_(L), r_(r) {}
    virtual double payoff(double S) const = 0;
};

class call : public option{

    public:
    call(double K, double L, double r): option(K, L, r) {}
    double payoff(double S) const override {
        if (S>L_){
            throw std::out_of_range("S is out of range");
        }
        return std::max(S - K_, 0.0);
    }
};

class put : public option{
    public:
    put(double K, double L, double r): option(K, L, r) {}
    double payoff(double S) const override {
        if (S>L_){
            throw std::out_of_range("S is out of range");
        }
        return std::max(K_ - S, 0.0);
    }
};



#endif // PAYOFF_HPP
