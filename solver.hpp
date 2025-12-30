#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include "edp.hpp"


class Solver {
    protected: //pour que les classes filles y aient accès
        double dt_; // Pas de temps
        double dS_; // Pas d'espace
        EDP&  edp_; // Référence vers l'EDP à résoudre
        int N_; // Nombre de points en espace
        int M_; // Nombre de points en temps
        std::vector<double> t_; // Valeurs de temps t pour lesquelles on calcule la solution
        std::vector<double> S_; // Valeurs de l'actif S pour lesquelles on calcule la solution
        std::vector<std::vector<double>> V_; // Matrice des prix

    public:
        Solver(EDP& edp, int N, int M);
        //     : t_(t), S_(S) {
        //     dt_ = T / M;
        //     dS_ = L / N;
        // }
        virtual void solve() = 0;
        ~Solver() = default;
    };


//classe Differences finies implicites
class solver_edp_reduite : public Solver { //heritage de la classe Solver
    public:
        solver_edp_reduite(EDP& edp, int N, int M);
            // : Solver(edp, N, M) {}
        void solve() override;
    };



//classe Cranck-Nicolson 
class solver_edp_complete : public Solver { //heritage de la classe Solver
    public:
        solver_edp_complete(EDP& edp, int N, int M);
            // : Solver(edp, N, M) {}
        void solve() override;
    };

#endif // SOLVER_HPP