#include "solver.hpp"
#include <cmath>
#include <algorithm>


//Implémentation des méthodes de la classe Solver
Solver::Solver(EDP& edp, int N, int M) : edp_(edp), N_(N), M_(M) {}
std::vector<double> algoThomas(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z, const std::vector<double>& b)
{
    // Taille du système
    int n = b.size();

    // Vecteurs temporaires utilisés par l'algorithme de Thomas
    std::vector<double> c(n);
    std::vector<double> d(n);

    // On effectue la décomposition LU de la matrice A en utilisant l'algorithme de Thomas
    c[0] = z[0] / y[0];
    d[0] = b[0] / y[0];
    for (int i = 1; i < n; i++)
    {
        c[i] = z[i] / (y[i] - x[i] * c[i-1]);
        d[i] = (b[i] - x[i] * d[i-1]) / (y[i] - x[i] * c[i-1]);
    }

    // Vecteur sol de la solution du système A * sol = b
    std::vector<double> sol(n);

    // On résout le système A * sol = b en utilisant la décomposition LU
    sol[n-1] = d[n-1];
    for (int i = n-2; i >= 0; i--)
    {
        sol[i] = d[i] - c[i] * sol[i+1];
    }

    return sol;
}

solver_edp_reduite::solver_edp_reduite(EDP& edp, int N, int M) : Solver(edp, N, M){
    //on récupère T, L, r et K de l'EDP
    double T = edp_.getT(); 
    double L = edp_.getL();
    double r = edp_.getR();
    double K = edp_.getOption()->get_strike_price();
    dt_ = T / static_cast<double>(M); //on convertit en double pour éviter les erreurs d'arrondis
    dS_ = L / static_cast<double>(N); //on convertit en double pour éviter les erreurs d'arrondis
    //Initialisation des vecteurs t_ et S_
    t_.resize(M_ + 1);
    for (int m = 0; m <= M_; ++m) {
        t_[m] = T - m*dt_;   
    }
    S_.resize(N_ + 1);
    for (int n = 0; n <= N_; ++n) {
        S_[n] = std::log((n*dS_)/K);
    } 
    //Initialisation de la matrice V_ de taille (M_+1) x (N_+1)
    V_.resize(M_ + 1, std::vector<double>(N_ + 1, 0.0));    //remplie de 0.0
    //Conditions aux limites
    for (int t = 0; t < M_; ++t) {
        V_[t][0] = (edp_.getOption()->boundary_condition_low(t_[t])) / (K*std::exp(-r*t_[t]));      //Condition aux limites S=0
        V_[t][N_] = (edp_.getOption()->boundary_condition_high(t_[t])) / (K*std::exp(-r*t_[t]));    //Condition aux limites S=L
    }
    //Condition initiale à t=T (m=M_)
    for(int s = 0; s <= N_; ++s) {
        V_[0][s] = edp_.getOption()->payoff(S_[s]) / K;
    }
    
}
//Implémentation des méthodes de la classe solver_edp_reduite
void solver_edp_reduite::reverse_variables() {
    double T = edp_.getT();
    double r = edp_.getR();
    double K = edp_.getOption()->get_strike_price();

    for (int s=0; s <=N_; s++){
        V_[0][s]= K*std::exp(-r*T)*V_[0][s];
    }
}
std::vector<double> solver_edp_reduite::solve() {

    double c=(std::pow(edp_.getSigma(),2)*0.5*dt_)/(std::pow(dS_,2));
    for (int s= 1; s < N_; ++s) {
        for(int n = M_; n >= 0; --n) {
            V_[n][s] = -c*V_[n-1][s-1] + (1+2*c)*V_[n-1][s] - c*V_[n-1][s+1];
        }
    }
    reverse_variables();
    return V_[0];
}

solver_edp_complete::solver_edp_complete(EDP& edp, int N, int M) : Solver(edp, N, M) {
    //on récupère T et L de l'EDP
    double T= edp_.getT(); 
    double L= edp_.getL();
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
    for (int t = 0; t < M_; ++t) {
        V_[t][0] = edp_.getOption()->boundary_condition_low(t_[t]);      //Condition aux limites S=0
        V_[t][N_] = edp_.getOption()->boundary_condition_high(t_[t]);    //Condition aux limites S=L
    }
    //Condition initiale à t=T (m=M_)
    for (int s = 0; s <= N_; ++s) {
        V_[M_][s] = edp_.getOption()->payoff(S_[s]);
    }
} 

//Implémentation des méthodes de la classe solver_edp_complete
std::vector<double> solver_edp_complete::solve() {


    
    return V_[0];
}