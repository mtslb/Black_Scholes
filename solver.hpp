/**
 * @file solver.hpp
 * @brief Moteur de résolution numérique (Crank-Nicolson et Implicite).
 */
#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include "payoff.hpp"

/**
 * @class BSSolver
 * @brief Gère la résolution des EDP complète (1) et réduite (5).
 */
class BSSolver {
public:
    /**
     * @brief Constructeur avec paramètres financiers.
     * @param r Taux d'intérêt.
     * @param sigma Volatilité de l'actif.
     * @param T Temps terminal.
     * @param L Borne supérieure de l'actif.
     * @param K Strike de l'option.
     */
    BSSolver(double r, double sigma, double T, double L, double K);

    /**
     * @brief Résout l'EDP complète (1) via Crank-Nicolson.
     * @param p Instance de Payoff (Call ou Put).
     * @param M Nombre d'intervalles de temps.
     * @param N Nombre d'intervalles d'espace.
     */
    std::vector<double> solve_complete(const Payoff& p, int M, int N);

    /**
     * @brief Résout l'EDP réduite (5) par changement de variable.
     * @param p Instance de Payoff.
     * @param M Pas de temps.
     * @param N Pas d'espace.
     */
    std::vector<double> solve_reduced(const Payoff& p, int M, int N);

private:
    double r, sigma, T, L, K;

    /** @brief Algorithme de Thomas pour système tridiagonal. */
    void tridiagonal_solve(const std::vector<double>& a, std::vector<double>& b, 
                           const std::vector<double>& c, std::vector<double>& rhs);
};

#endif // SOLVER_HPP