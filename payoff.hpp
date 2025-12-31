/**
 * @file payoff.hpp
 * @brief Définition de la hiérarchie des classes pour le calcul du Payoff.
 */
#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include <algorithm>
#include <cmath>

/**
 * @class Payoff
 * @brief Interface abstraite pour les différents types d'options (Call/Put).
 */
class Payoff {
public:
    virtual ~Payoff() {}
    virtual double operator()(double s) const = 0;
    virtual double get_boundary_zero(double t, double T, double r, double K) const = 0;
    virtual double get_boundary_L(double t, double T, double r, double K, double L) const = 0;
};

class CallPayoff : public Payoff {
    double K;
public:
    CallPayoff(double strike) : K(strike) {}
    double operator()(double s) const override;
    double get_boundary_zero(double t, double T, double r, double K) const override;
    double get_boundary_L(double t, double T, double r, double K, double L) const override;
};

class PutPayoff : public Payoff {
    double K;
public:
    PutPayoff(double strike) : K(strike) {}
    double operator()(double s) const override;
    double get_boundary_zero(double t, double T, double r, double K) const override;
    double get_boundary_L(double t, double T, double r, double K, double L) const override;
};

#endif // PAYOFF_HPP