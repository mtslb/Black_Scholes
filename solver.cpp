#include "solver.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

// ALGORITHME DE THOMAS SÉCURISÉ
std::vector<double> solver::thomas_algorithm(const std::vector<double>& a, 
                                            const std::vector<double>& b, 
                                            const std::vector<double>& c, 
                                            const std::vector<double>& d) {
    int n = d.size();
    std::vector<double> cp(n, 0.0);
    std::vector<double> dp(n, 0.0);
    std::vector<double> sol(n, 0.0);

    // Phase descendante
    cp[0] = c[0] / b[0];
    dp[0] = d[0] / b[0];

    for (int i = 1; i < n; i++) {
        double denom = b[i] - a[i] * cp[i - 1];
        // Sécurité contre division par zéro si r ou sigma sont instables
        if (std::abs(denom) < 1e-20) denom = 1e-20; 
        cp[i] = c[i] / denom;
        dp[i] = (d[i] - a[i] * dp[i - 1]) / denom;
    }

    // Phase ascendante
    sol[n - 1] = dp[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        sol[i] = dp[i] - cp[i] * sol[i + 1];
    }
    return sol;
}

solver::solver(EDP& edp, int N, int M) : edp_(edp), N_(N), M_(M) {
    dt_ = edp_.getT() / static_cast<double>(M_);
    dS_ = edp_.getL() / static_cast<double>(N_);

    S_.resize(N_ + 1);
    for (int i = 0; i <= N_; ++i) S_[i] = i * dS_;

    t_.resize(M_ + 1);
    for (int j = 0; j <= M_; ++j) t_[j] = j * dt_;

    v_.resize(M_ + 1, std::vector<double>(N_ + 1, 0.0));
}

void cranck_nicolson::solve() {
    double r = edp_.getR();
    double sigma = edp_.getSigma();

    // Condition terminale (Payoff)
    for (int i = 0; i <= N_; ++i) {
        v_[M_][i] = edp_.getOption()->payoff(S_[i]);
    }

    // Taille du système interne : N-1 points
    int n_size = N_ - 1;
    std::vector<double> a(n_size), b(n_size), c(n_size), d(n_size);

    std::cout << "Lancement de la boucle temporelle (M=" << M_ << ")..." << std::endl;

    for (int j = M_ - 1; j >= 0; --j) {
        // Affichage du progrès tous les 100 pas pour vérifier que ça ne bloque pas
        if (j % 100 == 0) std::cout << "Calcul etape t = " << j << std::endl;

        for (int i = 1; i < N_; ++i) {
            double s_i = S_[i];
            double sigma2_s2 = sigma * sigma * s_i * s_i;
            
            // Coefficients Crank-Nicolson
            double alpha = 0.25 * dt_ * (sigma2_s2 / (dS_ * dS_) - r * s_i / dS_);
            double beta  = -0.5 * dt_ * (sigma2_s2 / (dS_ * dS_) + r);
            double gamma = 0.25 * dt_ * (sigma2_s2 / (dS_ * dS_) + r * s_i / dS_);

            // Matrice de gauche
            a[i - 1] = -alpha;
            b[i - 1] = 1.0 - beta;
            c[i - 1] = -gamma;

            // Membre de droite
            d[i - 1] = alpha * v_[j + 1][i - 1] + (1.0 + beta) * v_[j + 1][i] + gamma * v_[j + 1][i + 1];
        }

        // Conditions aux limites
        v_[j][0] = edp_.getOption()->boundary_condition_low(t_[j]);
        v_[j][N_] = edp_.getOption()->boundary_condition_high(t_[j]);

        // Correction des bords pour Thomas
        double s1 = S_[1];
        double alpha1 = 0.25 * dt_ * (sigma * sigma * s1 * s1 / (dS_ * dS_) - r * s1 / dS_);
        d[0] += alpha1 * v_[j][0];

        double sN = S_[N_-1];
        double gammaN = 0.25 * dt_ * (sigma * sigma * sN * sN / (dS_ * dS_) + r * sN / dS_);
        d[n_size - 1] += gammaN * v_[j][N_];

        // Résolution
        std::vector<double> solution = thomas_algorithm(a, b, c, d);
        for (int i = 1; i < N_; ++i) {
            v_[j][i] = solution[i - 1];
        }
    }
    std::cout << "Calcul du solver TERMINE avec succes." << std::endl;
}

void implicite_solver::solve() {
    // Pour l'EDP réduite (Équation de la chaleur : u_t = 0.5 * sigma^2 * u_xx)
    double lambda = (edp_.getSigma() * edp_.getSigma() * dt_) / (2 * dS_ * dS_);

    // Initialisation Payoff
    for (int i = 0; i <= N_; ++i) v_[M_][i] = edp_.getOption()->payoff(S_[i]);

    std::vector<double> a(N_ - 1, -lambda);
    std::vector<double> b(N_ - 1, 1 + 2 * lambda);
    std::vector<double> c(N_ - 1, -lambda);
    std::vector<double> d(N_ - 1);

    for (int j = M_ - 1; j >= 0; --j) {
        for (int i = 1; i < N_; ++i) {
            d[i - 1] = v_[j + 1][i];
        }

        // Conditions bords (selon ton EDP réduite)
        v_[j][0] = edp_.getOption()->boundary_condition_low(t_[j]);
        v_[j][N_] = edp_.getOption()->boundary_condition_high(t_[j]);

        d[0] += lambda * v_[j][0];
        d[N_ - 2] += lambda * v_[j][N_];

        std::vector<double> sol = thomas_algorithm(a, b, c, d);
        for (int i = 1; i < N_; ++i) v_[j][i] = sol[i - 1];
    }
}
