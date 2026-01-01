/**
 * @file edp.hpp
 * @brief Définition des classes représentant les Équations aux Dérivées Partielles (EDP).
 */

#ifndef EDP_HPP
#define EDP_HPP

#include "payoff.hpp"

/**
 * @class EDP
 * @brief Classe abstraite modélisant une EDP de Black-Scholes.
 */
class EDP {
    protected:
        Option* option_; /**< Pointeur vers l'objet Option associé */
        double sigma_;   /**< Volatilité du sous-jacent */
        double r_;       /**< Taux d'intérêt sans risque */
        double T_;       /**< Temps terminal */
        double L_;       /**< Valeur maximale de l'actif */

    public:
        /**
         * @brief Constructeur de la classe EDP.
         * @param option Pointeur vers l'option.
         * @param sigma Volatilité.
         * @param r Taux d'intérêt.
         * @param T Échéance.
         * @param L Borne supérieure de S.
         */
        EDP(Option* option, double sigma, double r, double T, double L);

        /** @brief Destructeur virtuel par défaut. */
        virtual ~EDP() = default;

        /** @return Pointeur vers l'option associée. */
        Option* getOption() const;
        /** @return Valeur de la volatilité. */
        double getSigma() const;
        /** @return Valeur du taux d'intérêt. */
        double getR() const;
        /** @return Valeur du temps terminal. */
        double getT() const;
        /** @return Valeur de la borne spatiale supérieure. */
        double getL() const;
};

/**
 * @class EDP_complete
 * @brief Modèle l'EDP de Black-Scholes complète.
 */
class EDP_complete : public EDP {
    public:
        EDP_complete(Option* option, double sigma, double r, double T, double L);
};

/**
 * @class EDP_reduite
 * @brief Modèle l'EDP de Black-Scholes sous sa forme réduite (changement de variable).
 */
class EDP_reduite : public EDP {
    public:
        EDP_reduite(Option* option, double sigma, double r, double T, double L);
};

#endif // EDP_HPP