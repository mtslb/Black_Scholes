/**
 * @file solver.cpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Implémentation de la classe abstraite Solver et de ses classes dérivées Cranck-Nicolson et Implicite
 */


#include "solver.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>


/**
 * @brief Constructeur de la classe Solver
 * @param edp Référence vers l'EDP à résoudre
 * @param N Nombre de points en espace
 * @param M Nombre de points en temps
 */
Solver::Solver(EDP& edp, int N, int M) : edp_(edp), N_(N), M_(M) {  
    dt_ = edp_.getT() / static_cast<double>(M_); //pas de temps
    dS_ = edp_.getL() / static_cast<double>(N_); //pas en espace

    S_.resize(N_ + 1);
    for (int i = 0; i <= N_; ++i) S_[i] = i * dS_;
    t_.resize(M_ + 1);
    for (int j = 0; j <= M_; ++j) t_[j] = j * dt_;

    v_.resize(M_ + 1, std::vector<double>(N_ + 1, 0.0));
}

/**
 * @brief Getter pour récupérer les résultats
 * @return Matrice des solutions (valeurs de l'option)
 */
std::vector<std::vector<double>> Solver::get_results() const {
    return v_;
}

/**
 * @brief Algorithme de Thomas pour résoudre un système tridiagonal
 * @param a Diagonale inférieure
 * @param b Diagonale principale
 * @param c Diagonale supérieure
 * @param d Membre de droite
 * @return Solution du système tridiagonal
 */
std::vector<double> Solver::thomas_algorithm(const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c, const std::vector<double>& d) {
    int n = d.size();  //taille du système
    std::vector<double> cp(n, 0.0); //coefficients modifiés
    std::vector<double> dp(n, 0.0); //membre de droite modifié
    std::vector<double> sol(n, 0.0); //solution

    //Eliminer les coefficients a_i sous la diagonale pour transformer la matrice en une matrice triangulaire supérieure
    cp[0] = c[0] / b[0];
    dp[0] = d[0] / b[0]; 

    for (int i = 1; i < n; i++) {
        double denom = b[i] - a[i] * cp[i - 1];  
        if (std::abs(denom) < 1e-20) {
            denom = 1e-20;  
        }
        cp[i] = c[i] / denom;
        dp[i] = (d[i] - a[i] * dp[i - 1]) / denom;
    }

    //La solution pour le dernier prix de l'action est simplement le dernier coefficient modifié 
    sol[n - 1] = dp[n - 1];
    // remonter la matrice triangulaire supérieure pour trouver les autres valeurs de la solution
    for (int i = n - 2; i >= 0; i--) {
        sol[i] = dp[i] - cp[i] * sol[i + 1];
    }
    return sol;
}

/**
 * @brief Constructeur de la classe Cranck_nicolson
 * @param edp Référence vers l'EDP à résoudre
 * @param N Nombre de points en espace
 * @param M Nombre de points en temps
 */
Cranck_nicolson::Cranck_nicolson(EDP& edp, int N, int M) : 
    Solver(edp, N, M) {}

/** 
 * @brief Méthode de résolution de l'équation de Black-Scholes avec Crank-Nicolson
 */

void Cranck_nicolson::solve() {
    double r = edp_.getR();  //on récupère le taux d'intérêt
    double sigma = edp_.getSigma(); //on récupère la volatilité

    //initialise la dernière ligne de la matrice v avec le payoff
    for (int i = 0; i <= N_; ++i) {
        v_[M_][i] = edp_.getOption()->payoff(S_[i]);   //payoff de call ou put
    }

    // Taille du système interne : N-1 points
    int n_size = N_ - 1;
    std::vector<double> a(n_size), b(n_size), c(n_size), d(n_size);


    for (int j = M_ - 1; j >= 0; --j) { //parcours le temps à l'envers
        for (int i = 1; i < N_; ++i) {  //pour chaque prix de l'actif
            double s_i = S_[i];
            double sigma2_s2 = sigma * sigma * s_i * s_i;
            
            //coefficients de crank-nicolson
            double alpha = 0.25 * dt_ * (sigma2_s2 / (dS_ * dS_) - r * s_i / dS_);
            double beta  = -0.5 * dt_ * (sigma2_s2 / (dS_ * dS_) + r);
            double gamma = 0.25 * dt_ * (sigma2_s2 / (dS_ * dS_) + r * s_i / dS_);

            // Matrice tridiagonale
            a[i - 1] = -alpha;
            b[i - 1] = 1.0 - beta;
            c[i - 1] = -gamma;

            //membre de droite calculé à partir des prix à l'instant j+1
            d[i - 1] = alpha * v_[j + 1][i - 1] + (1.0 + beta) * v_[j + 1][i] + gamma * v_[j + 1][i + 1];
        }

        //conditions aux limites
        v_[j][0] = edp_.getOption()->boundary_condition_low(t_[j]); //condition à la frontière basse
        v_[j][N_] = edp_.getOption()->boundary_condition_high(t_[j]); //condition à la frontière haute

        //conditions aux limites dans le membre de droite
        double s1 = S_[1];
        double alpha1 = 0.25 * dt_ * (sigma * sigma * s1 * s1 / (dS_ * dS_) - r * s1 / dS_);
        d[0] += alpha1 * v_[j][0]; //ajout de la condition à la frontière basse

        double sN = S_[N_-1];
        double gammaN = 0.25 * dt_ * (sigma * sigma * sN * sN / (dS_ * dS_) + r * sN / dS_);
        d[n_size - 1] += gammaN * v_[j][N_]; //ajout de la condition à la frontière haute

        //résolution du système tridiagonal
        std::vector<double> solution = thomas_algorithm(a, b, c, d);
        for (int i = 1; i < N_; ++i) {
            v_[j][i] = solution[i - 1];
        }
    }
}

/**
 * @brief Constructeur de la classe Implicite_solver
 * @param edp Référence vers l'EDP à résoudre
 * @param N Nombre de points en espace
 * @param M Nombre de points en temps
 */
Implicite_solver::Implicite_solver(EDP& edp, int N, int M) : 
    Solver(edp, N, M) {}

/** @brief Méthode de résolution de l'équation de Black-Scholes avec méthode implicite 
*/
void Implicite_solver::solve() {
    //pour l'EDP réduite (équation de la chaleur : u_t = 0.5 * sigma^2 * u_xx)
    double lambda = (edp_.getSigma() * edp_.getSigma() * dt_) / (2 * dS_ * dS_);

    //initialisation Payoff 
    for (int i = 0; i <= N_; ++i) v_[M_][i] = edp_.getOption()->payoff(S_[i]);

    std::vector<double> a(N_ - 1, -lambda);  //diagonale inférieure
    std::vector<double> b(N_ - 1, 1 + 2 * lambda); //diagonale principale
    std::vector<double> c(N_ - 1, -lambda); //diagonale supérieure
    std::vector<double> d(N_ - 1); //membre de droite

    for (int j = M_ - 1; j >= 0; --j) {
        for (int i = 1; i < N_; ++i) {
            d[i - 1] = v_[j + 1][i]; //membre de droite=prix à l'instant j+1
        }

        //Conditions aux bords
        v_[j][0] = edp_.getOption()->boundary_condition_low(t_[j]);   //condition à la frontière basse
        v_[j][N_] = edp_.getOption()->boundary_condition_high(t_[j]);   //condition à la frontière haute

        d[0] += lambda * v_[j][0]; //ajout de la condition à la frontière basse
        d[N_ - 2] += lambda * v_[j][N_];  //ajout de la condition à la frontière haute

        //résolution du système tridiagonal
        std::vector<double> sol = thomas_algorithm(a, b, c, d);
        for (int i = 1; i < N_; ++i) {
            v_[j][i] = sol[i - 1];
        }
    }
}