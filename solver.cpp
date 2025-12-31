/**
 * @file solver.cpp
 * @brief Implémentation des schémas Crank-Nicolson et Implicite.
 */
#include "solver.hpp"
#include <cmath>
#include <vector>

BSSolver::BSSolver(double r, double sigma, double T, double L, double K) 
    : r(r), sigma(sigma), T(T), L(L), K(K) {}

/**
 * @brief Algorithme de Thomas pour systèmes tridiagonaux.
 */
void BSSolver::tridiagonal_solve(const std::vector<double>& a, std::vector<double>& b, 
                                 const std::vector<double>& c, std::vector<double>& rhs) {
    int n = rhs.size();
    for (int i = 1; i < n; i++) {
        double m = a[i] / b[i-1];
        b[i] -= m * c[i-1];
        rhs[i] -= m * rhs[i-1];
    }
    rhs[n-1] /= b[n-1];
    for (int i = n - 2; i >= 0; i--) {
        rhs[i] = (rhs[i] - c[i] * rhs[i+1]) / b[i];
    }
}

/**
 * @brief Résolution de l'EDP complète (1) par Crank-Nicolson[cite: 79].
 */
std::vector<double> BSSolver::solve_complete(const Payoff& p, int M, int N) {
    double dt = T / M;
    double ds = L / N;
    std::vector<double> u(N + 1);
    for (int j = 0; j <= N; j++) u[j] = p(j * ds);

    for (int i = M - 1; i >= 0; i--) {
        std::vector<double> a(N-1), b(N-1), c(N-1), rhs(N-1);
        for (int j = 1; j < N; j++) {
            double s_j = j * ds;
            double sig2s2 = sigma * sigma * s_j * s_j;
            double alpha = 0.25 * dt * (sig2s2 / (ds * ds) - r * s_j / ds);
            double beta = -0.5 * dt * (sig2s2 / (ds * ds) + r);
            double gamma = 0.25 * dt * (sig2s2 / (ds * ds) + r * s_j / ds);

            a[j-1] = -alpha; b[j-1] = 1.0 - beta; c[j-1] = -gamma;
            rhs[j-1] = alpha * u[j-1] + (1.0 + beta) * u[j] + gamma * u[j+1];
        }
        tridiagonal_solve(a, b, c, rhs);
        for (int j = 1; j < N; j++) u[j] = rhs[j-1];
        u[0] = p.get_boundary_zero(i*dt, T, r, K);
        u[N] = p.get_boundary_L(i*dt, T, r, K, L);
    }
    return u;
}

/**
 * @brief Résolution de l'EDP réduite (5) par schéma implicite[cite: 83].
 */
std::vector<double> BSSolver::solve_reduced(const Payoff& p, int M, int N) {
    double dt = T / M;
    double ds = L / N;
    double diff_coeff = 0.5 * sigma * sigma; // Coeff de l'EDP réduite
    std::vector<double> u(N + 1);
    for (int j = 0; j <= N; j++) u[j] = p(j * ds);

    double lambda = diff_coeff * dt / (ds * ds);

    for (int i = M - 1; i >= 0; i--) {
        std::vector<double> a(N-1, -lambda), b(N-1, 1.0 + 2.0*lambda), c(N-1, -lambda), rhs(N-1);
        for (int j = 1; j < N; j++) rhs[j-1] = u[j];
        
        rhs[0] += lambda * p.get_boundary_zero(i*dt, T, r, K);
        rhs[N-2] += lambda * p.get_boundary_L(i*dt, T, r, K, L);

        tridiagonal_solve(a, b, c, rhs);
        for (int j = 1; j < N; j++) u[j] = rhs[j-1];
        u[0] = p.get_boundary_zero(i*dt, T, r, K);
        u[N] = p.get_boundary_L(i*dt, T, r, K, L);
    }
    return u;
}