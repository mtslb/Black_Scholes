/**
 * @file Solver.hpp
 * @brief Moteur de résolution numérique pour l'équation de Black-Scholes.
 */
#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include "payoff.hpp"

/**
 * @class BSSolver
 * @brief Gère la résolution de l'EDP complète et réduite.
 */
class BSSolver {
public:
    /**
     * @brief Constructeur du solveur.
     * @param r Taux d'intérêt.
     * @param sigma Volatilité.
     * @param T Maturité.
     * @param L Borne spatiale supérieure.
     * @param K Strike.
     */
    BSSolver(double r, double sigma, double T, double L, double K);
    
    /**
     * @brief Résout l'EDP complète par le schéma de Crank-Nicolson.
     * @param p Référence vers l'objet Payoff (Call ou Put).
     * @param M Nombre de pas de temps.
     * @param N Nombre de pas d'espace.
     * @return Vecteur contenant les prix à t=0 pour chaque s.
     */
    std::vector<double> solve_complete(const Payoff& p, int M, int N);
    
    /**
     * @brief Résout l'EDP réduite (équation 5 du sujet).
     * @param p Référence vers le Payoff.
     * @param M Nombre de pas de temps.
     * @param N Nombre de pas d'espace.
     * @return Vecteur des prix calculés.
     */
    std::vector<double> solve_reduced(const Payoff& p, int M, int N);

private:
    double r, sigma, T, L, K;

    /**
     * @brief Algorithme de Thomas pour résoudre un système tridiagonal.
     * @param a Diagonale inférieure.
     * @param b Diagonale principale (modifiée pendant le calcul).
     * @param c Diagonale supérieure.
     * @param rhs Second membre (modifié pour contenir la solution).
     */
    void tridiagonal_solve(const std::vector<double>& a, std::vector<double>& b, 
                           const std::vector<double>& c, std::vector<double>& rhs);
};

#endif // SOLVER_HPP