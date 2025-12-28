#include "solver.hpp"

Solver::Solver(EDP& edp, int N, int M) : edp_(edp), N_(N), M_(M) {
    //on récupère T et L de l'EDP
    int T= edp_.getT(); 
    int L= edp_.getL();

}