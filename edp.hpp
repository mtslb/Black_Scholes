#ifndef EDP_HPP
#define EDP_HPP

#include "payoff.hpp"


class EDP {  //classe abstraite EDP
    protected:
        Option* option_; // pointeur vers l'option (Call ou Put) 
        double sigma_; // Volatilité de l'actif sous-jacent
        double r_;     // Taux d'intérêt sans risque
        double T_;     // Temps terminal (double pour moins de conversion et précision)
        double L_;     // Valeur maximale de l'actif sous-jacent (pareil pour double)
    public:
        EDP(Option* option, double sigma, double r, double T, double L); //constructeur avec arguments pour être sur que l'objet existe par la suite
        virtual ~EDP() = default; // destructeur virtuel 
    //Getters pour que Solver puisse accéder aux attributs protégés
        Option* getOption() const;
        double getSigma() const;
        double getR() const;
        double getT() const;
        double getL() const;
};


class EDP_complete : public EDP{ //heritage de la classe EDP
    public :
        EDP_complete(Option* option, double sigma, double r, double T, double L);  //constructeur
};

class EDP_reduite: public EDP{
    public :
        EDP_reduite(Option* option, double sigma, double r, double T, double L);  //constructeur 
};


#endif // EDP_HPP