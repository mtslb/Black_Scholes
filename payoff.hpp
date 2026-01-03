/**
 * @file payoff.hpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Définition de la classe abstraite Option et de ses classes dérivées Call et Put
 */

#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include<algorithm>
#include<cmath>
#include<stdexcept>


/**
 * @brief Classe abstraite Option
 */

class Option{
    protected:
        double K_; //strike 
        double L_; //valeur maximale de l'actif sous-jacent
        double r_; //taux d'intérêt sans risque
        double T_; //temps terminal

    public:
        /**
        * @brief Constructeur de la classe Option
        * @param K Strike de l'option
        * @param L Valeur maximale de l'actif sous-jacent
        * @param r Taux d'intérêt sans risque
        * @param T Temps terminal
        */
        Option(double K, double L, double r, double T); 

        /**
        * @brief Destructeur de la classe Option
        */
        virtual ~Option(); // destructeur virtuel
        
        /**
        * @brief Méthode virtuelle pure pour le payoff de l'option
        * @param S Prix de l'actif sous-jacent
        * @return Valeur du payoff
        */
        virtual double payoff(double S) const = 0; 

        /**
        * @brief Méthode virtuelle pure pour la condition à la limite basse
        * @param t Temps
        * @return Valeur de la condition à la limite basse S=0
        */
        virtual double boundary_condition_low(double L, double t) const = 0; 

        /**
        * @brief Méthode virtuelle pure pour la condition à la limite haute
        * @param t Temps
        * @return Valeur de la condition à la limite haute
        */
        virtual double boundary_condition_high(double L,double t) const = 0; 
};



/**
 * @brief Classe Call qui hérite de la classe Option
 */

class Call : public Option{ // Classe dérivée de Option
    public:
        /**
         * @brief Constructeur de la classe Call
         * @param K Strike de l'option
         * @param L Valeur maximale de l'actif sous-jacent
         * @param r Taux d'intérêt sans risque
         * @param T Temps terminal
         */
        Call(double K, double L, double r, double T);

        /**
         * @brief Méthode pour le payoff de l'option Call
         * @param S Prix de l'actif sous-jacent
         * @return Valeur du payoff
         */
        double payoff(double S) const ;

        /**
         * @brief Méthode pour la condition à la limite basse
         * @param t Temps
         * @return Valeur de la condition à la limite basse S=0
         */
        double boundary_condition_low(double L, double t) const ;    
        /**
         * @brief Méthode pour la condition à la limite haute
         * @param t Temps
         * @return Valeur de la condition à la limite haute S=L
         */
        double boundary_condition_high(double L, double t) const ;  
};



/**
 * @brief Classe Put qui hérite de la classe Option
 */
class Put : public Option{  
    public:
        /**
         * @brief Constructeur de la classe Put
         * @param K Strike de l'option
         * @param L Valeur maximale de l'actif sous-jacent
         * @param r Taux d'intérêt sans risque
         * @param T Temps terminal
         */
        Put(double K, double L, double r, double T); 

        /**
         * @brief Méthode pour le payoff de l'option Put
         * @param S Prix de l'actif sous-jacent
         * @return Valeur du payoff
         */
        double payoff(double S) const ; 

        /**
         * @brief Méthode pour la condition à la limite basse
         * @param t Temps
         * @return Valeur de la condition à la limite basse S=0
         */
        double boundary_condition_low(double L, double t) const ;     

        /**
         * @brief Méthode pour la condition à la limite haute
         * @param t Temps
         * @return Valeur de la condition à la limite haute S=L
         */
        double boundary_condition_high(double L, double t) const ;    
};
#endif // PAYOFF_HPP
