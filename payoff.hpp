/**
 * @file payoff.hpp
 * @brief Définition des classes pour le calcul du payoff et des conditions aux limites des options.
 */

#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include <algorithm>
#include <cmath>
#include <stdexcept>

/**
 * @class Option
 * @brief Classe de base abstraite représentant une option financière.
 */
class Option {
    protected:
        double K_; /**< Prix d'exercice (Strike) */
        double L_; /**< Valeur maximale de l'actif sous-jacent (Borne supérieure) */
        double r_; /**< Taux d'intérêt sans risque */
        double T_; /**< Maturité (Temps restant jusqu'à l'exercice) */

    public:
<<<<<<< Updated upstream
        Option(double K, double L, double r, double T); // constructeur
        virtual ~Option(); // destructeur virtuel
        virtual double payoff(double S) const = 0; // méthode virtuelle pure (=0)
        virtual double boundary_condition_low(double t) const = 0; // méthode virtuelle pure (=0)
        virtual double boundary_condition_high(double t) const = 0; // méthode virtuelle pure (=0)
        double get_strike_price() const; // Getter pour K_
=======
        /**
         * @brief Constructeur de la classe Option.
         * @param K Prix d'exercice.
         * @param L Borne supérieure du domaine spatial.
         * @param r Taux d'intérêt.
         * @param T Temps à l'échéance.
         */
        Option(double K, double L, double r, double T);

        /**
         * @brief Destructeur virtuel de la classe Option.
         */
        virtual ~Option();

        /**
         * @brief Calcul du payoff à l'échéance.
         * @param S Valeur du sous-jacent.
         * @return Valeur du payoff.
         */
        virtual double payoff(double S) const = 0;

        /**
         * @brief Condition aux limites pour la borne inférieure (S=0).
         * @param t Temps actuel.
         * @return Valeur de la condition.
         */
        virtual double boundary_condition_low(double t) const = 0;

        /**
         * @brief Condition aux limites pour la borne supérieure (S=L).
         * @param t Temps actuel.
         * @return Valeur de la condition.
         */
        virtual double boundary_condition_high(double t) const = 0;
>>>>>>> Stashed changes
};

/**
 * @class Call
 * @brief Implémentation d'une option d'achat (Call).
 */
class Call : public Option {
    public:
        Call(double K, double L, double r, double T);
        double payoff(double S) const override;
        double boundary_condition_low(double t) const override;
        double boundary_condition_high(double t) const override;
};

/**
 * @class Put
 * @brief Implémentation d'une option de vente (Put).
 */
class Put : public Option {
    public:
        Put(double K, double L, double r, double T);
        double payoff(double S) const override;
        double boundary_condition_low(double t) const override;
        double boundary_condition_high(double t) const override;
};

#endif // PAYOFF_HPP