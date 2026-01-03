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
std::vector< std::vector<double> > Solver::get_results() const {
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

    // Taille du système interne : N-1 points car on a 2 conditions aux bords
    int n_size = N_ - 1;
    std::vector<double> a(n_size), b(n_size), c(n_size), d(n_size);


    for (int j = M_ - 1; j >= 0; --j) { //parcours le temps à l'envers
        for (int i = 1; i < N_; ++i) {  //pour chaque prix de l'actif
            double s_i = S_[i];
            double sigma2_s2 = sigma * sigma * s_i * s_i;
            
            //coefficients de crank-nicolson
            double alpha = 0.25 * dt_ * (sigma2_s2 / (dS_ * dS_) - r * s_i / dS_);
            double beta  = - 0.5 * dt_ * (sigma2_s2 / (dS_ * dS_) + r);
            double gamma = 0.25 * dt_ * (sigma2_s2 / (dS_ * dS_) + r * s_i / dS_);

            // Matrice tridiagonale
            a[i - 1] = -alpha;
            b[i - 1] = 1.0 - beta;
            c[i - 1] = -gamma;

            //membre de droite calculé à partir des prix à l'instant j+1
            d[i - 1] = alpha * v_[j + 1][i - 1] + (1.0 + beta) * v_[j + 1][i] + gamma * v_[j + 1][i + 1];
        }

        //conditions aux limites
        v_[j][0] = edp_.getOption()->boundary_condition_low(edp_.getL(), t_[j]); //condition à la frontière basse
        v_[j][N_] = edp_.getOption()->boundary_condition_high(edp_.getL(), t_[j]); //condition à la frontière haute

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
    Solver(edp, N, M) {
        s_min=0.00001; //pour le changement de variable car ln(0) diverge
    }
/**
 * @brief Changement de variables S_ (prix)  pour l'EDP réduite 
 */
void Implicite_solver::change_variable() {
    double T = edp_.getT();
    double r = edp_.getR();
    double sigma2 = edp_.getSigma() * edp_.getSigma();
    double drift = r - 0.5 * sigma2;
    //changement de variable temporel
    for (int j=0; j<=M_; ++j){
        t_[j]= T-j*dt_;
    }
    //nouveau pas en espace
    double x_min = std::log(s_min);
    double x_max = std::log(edp_.getL()) + drift * T;
    dS_ = (x_max - x_min) / static_cast<double>(N_);
    //nouveau vecteur des prix (logarithmique)
    for (int i = 0; i <= N_; ++i) {
        S_[i] = x_min + i * dS_; // S_ stocke maintenant x = ln(S) 
    }
}
/**
 * @brief Changement inverse des variables pour superposition des courbes
 */
void Implicite_solver::reverse_variable() {
    double r = edp_.getR();
    double T = edp_.getT();
    double sigma2 = edp_.getSigma() * edp_.getSigma();
    double drift = r - 0.5 * sigma2;

    for (int j = 0; j <= M_; ++j) {
        double tau = t_[j]; // t_ contient tau
        for (int i = 0; i <= N_; ++i) {
            //on repasse de u à V
            v_[j][i] *= std::exp(-r * tau);
            
            //on ajuste S uniquement pour la sortie finale (souvent à j = M_, donc t=0)
            if (j == M_) {
                // Le x de calcul correspond au ln(S) + drift*T
                // Donc S = exp(x - drift * T)
                S_[i] = std::exp(S_[i] - drift * T);
            }
        }
        //on remet le temps dans le bon sens
        t_[j] = T - tau; 
        
    }
}
/** @brief Méthode de résolution de l'équation de Black-Scholes avec méthode implicite 
*/
void Implicite_solver::solve() {
    double r =edp_.getR();
    double sigma2=edp_.getSigma() * edp_.getSigma();
    //changement de variables pour l'EDP réduite
    change_variable();
    //pour l'EDP réduite (équation de la chaleur : u_t = 0.5 * sigma^2 * u_xx)
    double lambda = (sigma2 * dt_) / (2.0 * dS_ * dS_);

    double drift = r - 0.5 * sigma2;
    double x_max = std::log(edp_.getL()) + drift * edp_.getT();

    //initialisation Payoff à l'instant t=0  et s=exp(s) car changement de variable 
    for (int i = 0; i <= N_; ++i) {
        v_[0][i] = edp_.getOption()->payoff(std::exp(S_[i]));
    }
    
    std::vector<double> a(N_ - 1, -lambda);  //diagonale inférieure
    std::vector<double> b(N_ - 1, 1 + 2 * lambda); //diagonale principale
    std::vector<double> c(N_ - 1, -lambda); //diagonale supérieure
    std::vector<double> d(N_ - 1); //membre de droite

    for (int j = 1; j <= M_; ++j) {
        
        double real_t_ = edp_.getT() - t_[j];

        // Conditions aux bords après changement de variable (voir 2.4.2 du rapport)
        v_[j][0] = edp_.getOption()->boundary_condition_low(x_max, real_t_) * std::exp(r * t_[j]); 
        v_[j][N_] = edp_.getOption()->boundary_condition_high(x_max, real_t_) * std::exp(r * t_[j]);

        d[0] +=  lambda * v_[j][0]; //ajout de la condition à la frontière basse
        d[N_-2] += lambda * v_[j][N_];  //ajout de la condition à la frontière haute

        for (int i = 1; i < N_-2; ++i) {
            d[i] = v_[j][i]; //membre de droite=prix à l'instant j
        }

        //résolution du système tridiagonal
        std::vector<double> sol = thomas_algorithm(a, b, c, d);
        for (int i = 1; i < N_; ++i) {
            v_[j][i] = sol[i - 1];
        }
    }
    reverse_variable();
}

/**
 * @brief Récupère la valeur de l'option pour un prix S précis par interpolation
 * @param s_target Le prix S que l'on cherche (ex: 100.0)
 * @param time_step L'indice de temps (généralement 0 pour t=0)
 */
double Implicite_solver::get_value_at_S(double s_target, int time_step) const {
    // S_ contient les prix transformés par reverse_variable() pour l'instant t=0
    if (s_target <= S_[0]) return v_[time_step][0];
    if (s_target >= S_[N_]) return v_[time_step][N_];

    // Trouver l'intervalle [S_[i], S_[i+1]] qui entoure s_target
    int i = 0;
    while (i < N_ && S_[i+1] < s_target) {
        i++;
    }

    // Interpolation linéaire entre le point i et i+1
    double s_inf = S_[i];
    double s_sup = S_[i+1];
    double v_inf = v_[time_step][i];
    double v_sup = v_[time_step][i+1];

    return v_inf + (s_target - s_inf) * (v_sup - v_inf) / (s_sup - s_inf);
}