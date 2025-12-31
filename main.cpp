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

    Put put_option (K,L,r,T);
    Call call_option (K,L,r,T);

    EDP::EDP EDP_reduite_PUT(put_option, sigma, r, T, L);
    solver_edp_reduite::solver_edp_reduite_PUT(EDP_reduite_PUT, N, M);
    solver_edp_complete::solver_edp_complete_PUT(EDP_complete_PUT, N, M);

    sdl::init();
    sdl::run();
    EDP EDP_reduite_CALL(call_option, sigma, r, T, L);
    solver_edp_reduite::solver_edp_reduite_CALL(EDP_reduite_CALL, N, M);
    solver_edp_complete::solver_edp_complete_CALL(EDP_complete_CALL, N, M);





    sdl::exit();




    return 0;
}     

