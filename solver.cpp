#include "solver.hpp"
#include <cmath>
#include <algorithm>


//Implémentation des méthodes de la classe Solver
Solver::Solver(EDP& edp, int N, int M) : edp_(edp), N_(N), M_(M) {
    //on récupère T et L de l'EDP
    int T= edp_.getT(); 
    int L= edp_.getL();
    dt_ = T / static_cast<double>(M); //on convertit en double pour éviter les erreurs d'arrondis
    dS_ = L / static_cast<double>(N); //on convertit en double pour éviter les erreurs d'arrondis
    //Initialisation des vecteurs t_ et S_
    t_.resize(M_ + 1);
    for (int m = 0; m <= M_; ++m) {
        t_[m] = m * dt_;   
    }
    S_.resize(N_ + 1);
    for (int n = 0; n <= N_; ++n) {
        S_[n] = n * dS_;
    }
    //Initialisation de la matrice V_ de taille (M_+1) x (N_+1)
    V_.resize(M_ + 1, std::vector<double>(N_ + 1, 0.0));    //remplie de 0.0

    //Conditions aux limites
    for (int t = 0; t <= M_; ++t) {
        V_[t][0] = edp_.getOption()->boundary_condition_low(t_[t]);      //Condition aux limites S=0
        V_[t][N_] = edp_.getOption()->boundary_condition_high(t_[t]);    //Condition aux limites S=L
    }

    {
    //Condition initiale à t=T (m=M_)
    for (int s = 0; s <= N_; ++s) {
        V_[M_][s] = edp_.getOption()->payoff(S_[s]);
    }
    }
}
solver_edp_reduite::solver_edp_reduite(EDP& edp, int N, int M) : Solver(edp, N, M){
    for (int m = 0; m <= M_; ++m) {
        t_[m] = edp_.getT() - t_[m];   
    }
    for (int n = 0; n <= N_; ++n) {
        S_[n] = std::log(S_[n]/edp_.getOption()->get_strike_price());
    } 
}
//Implémentation des méthodes de la classe solver_edp_reduite
void solver_edp_reduite::solve() {

    double c=(edp_.getSigma()*edp_.getSigma()*dt_)/(std::pow(dS_,2));
    for (int s= 1; s < N_; ++s) {
        for(int n = 0; n < M_; ++n) {
            V_[n][s] = -c*V_[n+1][s-1] + (1+2*c)*V_[n+1][s] - c*V_[n+1][s+1];
        }
    }

}

solver_edp_complete::solver_edp_complete(EDP& edp, int N, int M)
    : Solver(edp, N, M) {} //appel du constructeur de la classe mère

//Implémentation des méthodes de la classe solver_edp_complete
void solver_edp_complete::solve() {
    V_[0][0] = edp_.getOption()->boundary_condition_low(t_[0]);
    V_[0][N_] = edp_.getOption()->boundary_condition_high(t_[0]);

}