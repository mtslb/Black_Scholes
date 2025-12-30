#include <iostream>

#include "solver.hpp"
#include "edp.hpp"
#include "payoff.hpp"
#include "sdl.hpp"

int main() {
    double T=1;
    double r=0.1;
    double sigma=0.1;
    double K=100;
    double L=200;

    int M=1000;
    int N=1000;


    put_option Put(K,L,r,T);
    call_option Call(K,L,r,T);

    EDP_complete_PUT edp(put_option, sigma, r, T, L);
    EDP_reduite_PUT edp_red(put_option, sigma, r, T, L);

    EDP_complete_CALL edp_call(call_option, sigma, r, T, L);
    EDP_reduite_CALL edp_red_call(call_option, sigma, r, T, L);



    return 0;
}     

