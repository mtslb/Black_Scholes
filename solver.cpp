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
}

//Implémentation des méthodes de la classe Differences_finies_implicites
void Differences_finies_implicites::solve() {
    //à implémenter
}

//Implémentation des méthodes de la classe Cranck_nicolson
void Cranck_nicolson::solve() {
    //à implémenter
}