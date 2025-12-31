/**
 * @file solver.cpp
 * @brief Implémentation des méthodes numériques avec transformations complètes.
 */
#include "solver.hpp"
#include <cmath>
#include <algorithm>

BSSolver::BSSolver(double r, double sigma, double T, double L, double K) 
    : r(r), sigma(sigma), T(T), L(L), K(K) {}

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
 * @brief EDP Complète (1) : Schéma de Crank-Nicolson.
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
            double alpha = 0.25 * dt * (sigma*sigma*s_j*s_j/(ds*ds) - r*s_j/ds);
            double beta = -0.5 * dt * (sigma*sigma*s_j*s_j/(ds*ds) + r);
            double gamma = 0.25 * dt * (sigma*sigma*s_j*s_j/(ds*ds) + r*s_j/ds);

            a[j-1] = -alpha; b[j-1] = 1.0 - beta; c[j-1] = -gamma;
            rhs[j-1] = alpha*u[j-1] + (1.0 + beta)*u[j] + gamma*u[j+1];
        }
        tridiagonal_solve(a, b, c, rhs);
        for (int j = 1; j < N; j++) u[j] = rhs[j-1];
        u[0] = p.get_boundary_zero(i*dt, T, r, K);
        u[N] = p.get_boundary_L(i*dt, T, r, K, L);
    }
    return u;
}

/**
 * @brief EDP Réduite (5) : Intègre S = exp(x), t = T - t_tilde, C_tilde = exp(rt)C.
 */
std::vector<double> BSSolver::solve_reduced(const Payoff& p, int M, int N) {
    double dt = T / M;
    // x = ln(S)
    double x_min = std::log(0.01); 
    double x_max = std::log(L);
    double dx = (x_max - x_min) / N;
    
    double mu = 0.5 * sigma * sigma; 
    double lambda = mu * dt / (dx * dx);

    // Initialisation : Payoff au temps t=T (t_tilde=0)
    std::vector<double> u(N + 1);
    for (int j = 0; j <= N; j++) {
        u[j] = p(std::exp(x_min + j * dx)); 
    }

    // Résolution de dC_tilde/dt_tilde = mu * d2C_tilde/dS_tilde^2
    for (int i = 1; i <= M; i++) {
        std::vector<double> a(N-1, -lambda), b(N-1, 1.0 + 2.0*lambda), c(N-1, -lambda), rhs(N-1);
        for (int j = 1; j < N; j++) rhs[j-1] = u[j];

        // t_tilde augmente, donc t = T - t_tilde diminue
        double t_tilde = i * dt;
        double t_reel = T - t_tilde;

        // Application du changement C_tilde = exp(r * t_tilde) * C
        double factor = std::exp(r * t_tilde);
        rhs[0] += lambda * p.get_boundary_zero(t_reel, T, r, K) * factor;
        rhs[N-2] += lambda * p.get_boundary_L(t_reel, T, r, K, L) * factor;

        tridiagonal_solve(a, b, c, rhs);

        for (int j = 1; j < N; j++) u[j] = rhs[j-1];
        u[0] = p.get_boundary_zero(t_reel, T, r, K) * factor;
        u[N] = p.get_boundary_L(t_reel, T, r, K, L) * factor;
    }

    // Reverse Mapping : Interpolation et retour au domaine C réel
    // C = exp(-r * T) * C_tilde car à la fin i=M, donc t_tilde = T
    std::vector<double> res(N + 1);
    double ds = L / N;
    double inv_discount = std::exp(-r * T);

    for (int j = 0; j <= N; j++) {
        double s = j * ds;
        if (s <= 0.01) res[j] = u[0] * inv_discount;
        else {
            double x = std::log(s);
            int idx = (x - x_min) / dx;
            if (idx >= N) res[j] = u[N] * inv_discount;
            else {
                double weight = (x - (x_min + idx * dx)) / dx;
                double val_interp = (1.0 - weight) * u[idx] + weight * u[idx+1];
                res[j] = val_interp * inv_discount;
            }
        }
    }
    return res;
}