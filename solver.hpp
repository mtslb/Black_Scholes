/**
 * @file solver.hpp
 * @brief Classes pour la résolution numérique des EDP (Implicite, Crank-Nicolson).
 */

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include "edp.hpp"

/**
 * @class solver
 * @brief Classe de base pour les schémas de résolution numérique.
 */
class solver {
protected:
    EDP& edp_; /**< Référence vers l'EDP à résoudre */
    int N_;    /**< Nombre de pas de temps */
    int M_;    /**< Nombre de pas d'espace */
    double dt_; /**< Taille du pas de temps */
    double dS_; /**< Taille du pas d'espace */
    std::vector<double> S_; /**< Maillage spatial */
    std::vector<double> t_; /**< Maillage temporel */
    std::vector< std::vector<double> > v_; /**< Matrice des résultats (Prix) */

public:
    /**
     * @brief Constructeur du solver.
     * @param edp Référence vers l'EDP.
     * @param N Nombre d'itérations temporelles.
     * @param M Nombre d'itérations spatiales.
     */
    solver(EDP& edp, int N, int M);

    /**
     * @brief Méthode virtuelle pure lançant la résolution.
     */
    virtual void solve() = 0;

    /**
     * @brief Récupère la matrice des prix calculés.
     * @return v_ Un vecteur de vecteurs de doubles.
     */
    std::vector< std::vector<double> > get_results() const { return v_; }
    
    /**
     * @brief Résout un système tridiagonal par l'algorithme de Thomas.
     * @param a Diagonale inférieure.
     * @param b Diagonale principale.
     * @param c Diagonale supérieure.
     * @param d Vecteur du second membre.
     * @return Vecteur solution du système.
     */
    std::vector<double> thomas_algorithm(const std::vector<double>& a, 
                                        const std::vector<double>& b, 
                                        const std::vector<double>& c, 
                                        const std::vector<double>& d);
};

/**
 * @class cranck_nicolson
 * @brief Implémentation du schéma numérique de Crank-Nicolson.
 */
class cranck_nicolson : public solver {
public:
    cranck_nicolson(EDP& edp, int N, int M) : solver(edp, N, M) {}
    /** @brief Exécute l'algorithme de Crank-Nicolson. */
    void solve() override;
};

/**
 * @class implicite_solver
 * @brief Implémentation du schéma numérique totalement implicite.
 */
class implicite_solver : public solver {
public:
    implicite_solver(EDP& edp, int N, int M) : solver(edp, N, M) {}
    /** @brief Exécute l'algorithme de résolution implicite. */
    void solve() override;
};

#endif
