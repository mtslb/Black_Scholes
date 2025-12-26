#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include "edp.hpp"


class solver {
    private:
        double dt_; // Pas de temps
        double dS_; // Pas d'espace
        std::vector<double>& t_; // Valeurs de temps t pour lesquelles on calcule la solution
        std::vector<double>& S_; // Valeurs de l'actif S pour lesquelles on calcule la solution

    public:
        solver(int L, int N, int T, int M, std::vector<double>& t, std::vector<double>& S)
            : t_(t), S_(S) {
            dt_ = T / M;
            dS_ = L / N;
        }

        virtual void solve() = 0;
    };

class differences_finies_implicites : public solver {
    public:
        differences_finies_implicites(int L, int N, int T, int M, std::vector<double>& t, std::vector<double>& S)
            : solver(L, N, T, M, t, S) {}

        void solve() override;
    };

class cranck_nicolson : public solver {
    public:
        cranck_nicolson(int L, int N, int T, int M, std::vector<double>& t, std::vector<double>& S)
            : solver(L, N, T, M, t, S) {}

        void solve() override;
    };

#endif // SOLVER_HPP