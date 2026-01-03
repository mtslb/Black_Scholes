/**
 * @file solver.hpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Définition de la classe abstraite Solver et de ses classes dérivées Cranck-Nicolson et Implicite
 */

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "edp.hpp"
#include <vector>


/**
 * @brief Classe abstraite Solver
 */

class Solver {
protected:
    EDP& edp_;   // Référence vers l'EDP à résoudre
    int N_;      // Nombre de points en espace
    int M_;      // Nombre de points en temps
    double dt_;  // Pas de temps
    double dS_;  // Pas en espace
    std::vector<double> S_;     // vecteur des prix de l'actif
    std::vector<double> t_;     // vecteur des temps
    std::vector< std::vector<double> > v_; // Matrice des solutions (valeurs de l'option) 

public:
    /**
     * @brief Constructeur de la classe Solver
     * @param edp Référence vers l'EDP à résoudre
     * @param N Nombre de points en espace
     * @param M Nombre de points en temps
     */
    Solver(EDP& edp, int N, int M);   
    

    /**
     * @brief Méthode virtuelle pure pour résoudre l'EDP
     */
    virtual void solve() = 0;  
    
    /**
     * @brief Getter pour récupérer les résultats
     * @return Matrice des solutions (valeurs de l'option)
     */
    std::vector< std::vector<double> > get_results() const;
    
    /**
     * @brief Algorithme de Thomas pour résoudre un système tridiagonal
     * @param a Diagonale inférieure
     * @param b Diagonale principale
     * @param c Diagonale supérieure
     * @param d Membre de droite
     * @return Solution du système tridiagonal
     */
    std::vector<double> thomas_algorithm(const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c, const std::vector<double>& d);
};



/** 
 * @brief Classe Crank-Nicolson héritant de Solver
 */

class Cranck_nicolson : public Solver {  
public:
    /**
     * @brief Constructeur de la classe Cranck_nicolson
     * @param edp Référence vers l'EDP à résoudre
     * @param N Nombre de points en espace
     * @param M Nombre de points en temps
     */
    Cranck_nicolson(EDP& edp, int N, int M);  

    /**
     * @brief Méthode de résolution crank-nicolson
     */
    void solve() ;
};



/**
 * @brief Classe Implicite héritant de Solver
 */

class Implicite_solver : public Solver {  
protected:
    double s_min; //valeur minimale pour changement de variable car ln(0) diverge
public:
    /**
     * @brief Constructeur de la classe Implicite_solver
     * @param edp Référence vers l'EDP à résoudre
     * @param N Nombre de points en espace
     * @param M Nombre de points en temps
     */
    Implicite_solver(EDP& edp, int N, int M); 

    /**
     * @brief Changement des variables 
     * S_ (prix) et t_ (temps) pour l'EDP réduite 
     */
    void change_variable();

    /**
     * @brief Changement inverse des variables pour superposition des courbes
     */
    void reverse_variable();
    
    /**
     * @brief Méthode de résolution implicite
     */
    void solve() ;

    /**
     * @brief Récupère la valeur de l'option pour un prix S précis par interpolation
     * @param s_target Le prix S que l'on cherche (ex: 100.0)
     * @param time_step L'indice de temps (généralement 0 pour t=0)
     */
    double get_value_at_S(double s_target, int time_step) const; 
};


#endif