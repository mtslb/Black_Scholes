#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "edp.hpp"
#include <vector>

class solver {
protected:
    EDP& edp_;   // Référence vers l'EDP à résoudre
    int N_;      // Nombre de points en espace
    int M_;      // Nombre de points en temps
    double dt_;  // Pas de temps
    double dS_;  // Pas en espace
    std::vector<double> S_;     // vecteur des prix de l'actif
    std::vector<double> t_;     // vecteur des temps
    std::vector<std::vector<double>> v_; // Matrice des solutions (valeurs de l'option) 

public:
    //constructeur
    solver(EDP& edp, int N, int M);   
    //destructeur
    virtual void solve() = 0;   // Méthode virtuelle pure (=0)
    // Getter pour récupérer les résultats
    std::vector<std::vector<double>> get_results() const;
    
    //algorithme de Thomas pour résoudre un système tridiagonal
    std::vector<double> thomas_algorithm(const std::vector<double>& a, const std::vector<double>& b, const std::vector<double>& c, const std::vector<double>& d);
};


//Implémentation des classes dérivées 




//Classe Crank-Nicolson 
class cranck_nicolson : public solver {  //hérite de la classe solver
public:
    //constructeur
    cranck_nicolson(EDP& edp, int N, int M);  
    //méthode de résolution crank-nicolson
    void solve() override;
};



//Classe Implicite
class implicite_solver : public solver {  //hérite de la classe solver
public:
    //constructeur
    implicite_solver(EDP& edp, int N, int M); 
    //méthode de résolution implicite
    void solve() override;
};


#endif