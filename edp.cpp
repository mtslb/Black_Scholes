/**
 * @file edp.cpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Implémentation de la classe abstraite EDP et de ses classes dérivées EDP_complete et EDP_reduite
 */

#include "edp.hpp"

/**
 * @brief Constructeur de la classe EDP
 * @param option Pointeur vers l'option (Call ou Put)
 * @param sigma Volatilité de l'actif sous-jacent
 * @param r Taux d'intérêt sans risque
 * @param T Temps terminal
 * @param L Valeur maximale de l'actif sous-jacent
 */
EDP::EDP(Option* option, double sigma, double r, double T, double L)
    : option_(option), sigma_(sigma), r_(r), T_(T), L_(L) {} //constructeur

/**
 * @brief Getter pour l'option
 * @return Pointeur vers l'option
 */
Option* EDP::getOption() const {
    return option_;
}

/**
 * @brief Getter pour la volatilité
 * @return Volatilité de l'actif sous-jacent
 */
double EDP::getSigma() const {
    return sigma_;
}

/**
 * @brief Getter pour le taux d'intérêt
 * @return Taux d'intérêt sans risque
 */
double EDP::getR() const {
    return r_;
}

/**
 * @brief Getter pour le temps terminal
 * @return Temps terminal
 */
double EDP::getT() const {
    return T_;
}   

/**
 * @brief Getter pour la valeur maximale de l'actif
 * @return Valeur maximale de l'actif sous-jacent
 */
double EDP::getL() const {
    return L_;
}


//Implémentation des méthodes de la classe EDP_complete
EDP_complete::EDP_complete(Option* option, double sigma, double r, double T, double L)
    : EDP(option, sigma, r, T, L) {}     //appel du constructeur de la classe mère EDP

//Implémentation des méthodes de la classe EDP_reduite
EDP_reduite::EDP_reduite(Option* option, double sigma, double r, double T, double L)
    : EDP(option, sigma, r, T, L) {}    //appel du constructeur de la classe mère EDP
