#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "edp.hpp"
#include <vector>

class solver {
protected:
    EDP& edp_;
    int N_, M_;
    double dt_, dS_;
    std::vector<double> S_, t_;
    std::vector<std::vector<double>> v_;

public:
    solver(EDP& edp, int N, int M);
    virtual void solve() = 0;
    std::vector<std::vector<double>> get_results() const { return v_; }
    
    // L'algorithme de Thomas adapté
    std::vector<double> thomas_algorithm(const std::vector<double>& a, 
                                        const std::vector<double>& b, 
                                        const std::vector<double>& c, 
                                        const std::vector<double>& d);
};

class cranck_nicolson : public solver {
public:
    cranck_nicolson(EDP& edp, int N, int M) : solver(edp, N, M) {}
    void solve() override;
};

class implicite_solver : public solver {
public:
    implicite_solver(EDP& edp, int N, int M) : solver(edp, N, M) {}
    void solve() override;
};

#endif
