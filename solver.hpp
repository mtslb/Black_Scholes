#ifndef
#define SOLVER_HPP


class solver {
    private:
        EDP& edp_;  // Référence vers l'objet EDP à résoudre
        int M_;     // Nombre de pas de temps
        int N_;     // Nombre de pas d'espace
        double dt_; // Pas de temps
        double dS_; // Pas d'espace
        std::vector<double>& t_; // Valeurs de temps t pour lesquelles on calcule la solution
        std::vector<double>& S_; // Valeurs de l'actif S pour lesquelles on calcule la solution

    public:
        solver(EDP& edp, int M, int N, std::vector<double>& t, std::vector<double>& S)
            : edp_(edp), M_(M), N_(N), t_(t), S_(S) {
            dt_ = edp_.T() / M_;
            dS_ = (edp_.S_max() - edp_.S_min()) / N_;
        }

        virtual ~solver() = default;

        virtual void solve() = 0;
    };

class elements_finis : public solver {
    public:
        elements_finis(EDP& edp, int M, int N, std::vector<double>& t, std::vector<double>& S)
            : solver(edp, M, N, t, S) {}

        void solve() override;
    };

class cranck_nicolson : public solver {
    public:
        cranck_nicolson(EDP& edp, int M, int N, std::vector<double>& t, std::vector<double>& S)
            : solver(edp, M, N, t, S) {}

        void solve() override;
    };

#endif // SOLVER_HPP