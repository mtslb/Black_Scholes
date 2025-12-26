#ifndef     
#define EDP_HPP

class EDP {
    protected:
        double sigma_; // Volatilité de l'actif sous-jacent
        double r_;     // Taux d'intérêt sans risque
        int T_;     // Temps terminal
        int L_;     // Valeur maximale de l'actif sous-jacent
    public:
        virtual ~EDP() = default;
}


class EDP_complete : public EDP{

}

class EDP_reduite: public EDP{

    
}
#endif // EDP_HPP