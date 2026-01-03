/**
 * @file payoff.cpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Implémentation de la classe abstraite Option et de ses classes dérivées Call et Put
 */

#include "payoff.hpp"
#include <cmath>
#include <algorithm>

/**
 * @brief Constructeur de la classe Option
 * @param K Strike de l'option
 * @param L Valeur maximale de l'actif sous-jacent
 * @param r Taux d'intérêt sans risque
 * @param T Temps terminal
 */
Option::Option(double K, double L, double r, double T) 
    : K_(K), L_(L), r_(r), T_(T) {}

/**
 * @brief Destructeur de la classe Option
 */
Option::~Option() {} 

/**
 * @brief Constructeur de la classe Call
 * @param K Strike de l'option
 * @param L Valeur maximale de l'actif sous-jacent
 * @param r Taux d'intérêt sans risque
 * @param T Temps terminal
 */
Call::Call(double K, double L, double r, double T) 
    : Option(K, L, r, T) {} 

/**
 * @brief Méthode pour le payoff de l'option Call
 * @param S Prix de l'actif sous-jacent
 * @return Valeur du payoff
 */
double Call::payoff(double S) const { 
    return std::max(S - K_, 0.0);
}

/**
 * @brief Méthode pour la condition aux limites basse
 * @param t Temps
 * @return Valeur de la condition aux limites basse S=0
 */
double Call::boundary_condition_low(double L, double t) const { 
    return L*t*0.0; 
}

/**
 * @brief Méthode pour la condition aux limites haute
 * @param t Temps
 * @return Valeur de la condition aux limites haute S=L
 */
double Call::boundary_condition_high(double L, double t) const { 
    return L - K_ * std::exp(-r_ * (t -T_));
}

/**
 * @brief Constructeur de la classe Put
 * @param K Strike de l'option
 * @param L Valeur maximale de l'actif sous-jacent
 * @param r Taux d'intérêt sans risque
 * @param T Temps terminal
 */
Put::Put(double K, double L, double r, double T) 
    : Option(K, L, r, T) {} 

/**
 * @brief Méthode pour le payoff de l'option Put
 * @param S Prix de l'actif sous-jacent
 * @return Valeur du payoff
 */
double Put::payoff(double S) const { 
    return std::max(K_ - S, 0.0);  
}

/**
 * @brief Méthode pour la condition aux limites basse
 * @param t Temps
 * @return Valeur de la condition aux limites basse S=0
 */
double Put::boundary_condition_low(double L, double t) const { 
    return L * 0.0 + K_ * std::exp(-r_ *(T_ - t)); 
}

/**
 * @brief Méthode pour la condition aux limites haute
 * @param t Temps
 * @return Valeur de la condition aux limites haute S=L
 */
double Put::boundary_condition_high(double L, double t) const { 
    return L*t*0.0;
}