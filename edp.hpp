/**
 * @file edp.hpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Définition de la classe abstraite EDP et de ses classes dérivées EDP_complete et EDP_reduite
 */


#ifndef EDP_HPP
#define EDP_HPP

#include "payoff.hpp" //pour la déclaration de la classe payoff


/**
 * @brief Classe abstraite EDP
 */

class EDP { 
    protected:
        Option* option_; // pointeur vers l'option (Call ou Put) 
        double sigma_; // Volatilité de l'actif sous-jacent
        double r_;     // Taux d'intérêt sans risque
        double T_;     // Temps terminal 
        double L_;     // Valeur maximale de l'actif sous-jacent

    public:
        /**
        * @brief Constructeur de la classe EDP
        * @param option Pointeur vers l'option (Call ou Put)
        * @param sigma Volatilité de l'actif sous-jacent
        * @param r Taux d'intérêt sans risque
        * @param T Temps terminal
        * @param L Valeur maximale de l'actif sous-jacent
        */
        EDP(Option* option, double sigma, double r, double T, double L); 

        /**
        * @brief Getter pour l'option
        * @return Pointeur vers l'option
        */
        Option* getOption() const;

        /**
        * @brief Getter pour la volatilité
        * @return Volatilité de l'actif sous-jacent
        */
        double getSigma() const;
        
        /**
        * @brief Getter pour le taux d'intérêt
        * @return Taux d'intérêt sans risque
        */
        double getR() const;

        /**
        * @brief Getter pour le temps terminal
        * @return Temps terminal
        */
        double getT() const;

        /**
        * @brief Getter pour la valeur maximale de l'actif
        * @return Valeur maximale de l'actif sous-jacent
        */
        double getL() const;
};


#endif // EDP_HPP
