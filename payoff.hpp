#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include<algorithm>

class option{
    protected:
    int K_;
    int L_;
    int r_;

    public:
    option(int K, int L, int r): K_(K), L_(L), r_(r) {}
    virtual ~option() = default;
    virtual double payoff(double S) const = 0;
};

class call : public option{
    public:
    call(int K, int L, int r): option(K, L, r) {}
    double payoff(double S) const override {
        return std::max(S - K_, 0.0);
    }
};

class put : public option{
    public:
    put(int K, int L, int r): option(K, L, r) {}
    double payoff(double S) const override {
        return std::max(K_ - S, 0.0);
    }
};



#endif // PAYOFF_HPP