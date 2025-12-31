/**
 * @file payoff.cpp
 * @brief Implémentation des méthodes liées aux conditions aux limites.
 */
#include "payoff.hpp"

double CallPayoff::operator()(double s) const {
    return std::max(s - K, 0.0);
}

double CallPayoff::get_boundary_zero(double t, double T, double r, double K) const {
    return 0.0;
}

double CallPayoff::get_boundary_L(double t, double T, double r, double K, double L) const {
    return L - K * std::exp(-r * (T - t));
}

double PutPayoff::operator()(double s) const {
    return std::max(K - s, 0.0);
}

double PutPayoff::get_boundary_zero(double t, double T, double r, double K) const {
    return K * std::exp(-r * (T - t));
}

double PutPayoff::get_boundary_L(double t, double T, double r, double K, double L) const {
    return 0.0;
}