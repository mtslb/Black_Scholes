/**
 * @file main.cpp
 * @brief Affichage séquentiel des courbes de prix et d'erreur (Consigne 3.3).
 * @details Ce programme affiche successivement :
 * 1. Put : Méthode Complète vs Réduite superposées.
 * 2. Erreur absolue pour le Put.
 * 3. Call : Méthode Complète vs Réduite superposées.
 * 4. Erreur absolue pour le Call.
 */

#include <iostream>
<<<<<<< Updated upstream

=======
#include <vector>
#include <cmath>
#include <string>
#include "payoff.hpp"
#include "edp.hpp"
>>>>>>> Stashed changes
#include "solver.hpp"
#include "edp.hpp"
#include "payoff.hpp"
#include "sdl.hpp"

<<<<<<< Updated upstream
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

=======
/**
 * @brief Calcule l'erreur absolue entre deux vecteurs de prix.
 * @param v1 Premier vecteur (ex: méthode complète).
 * @param v2 Second vecteur (ex: méthode réduite).
 * @return Vecteur contenant |v1[i] - v2[i]|.
 */
std::vector<double> compute_error(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> res(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        res[i] = std::abs(v1[i] - v2[i]);
    }
    return res;
}

/**
 * @brief Gère l'affichage d'une ou deux courbes et attend la fermeture.
 */
void display_block(Sdl& graphique, SDL_Renderer* renderer, const std::vector<double>& S, 
                   const std::vector<double>& v1, const std::vector<Uint8>& col1,
                   const std::vector<double>& v2, const std::vector<Uint8>& col2,
                   bool double_plot, const std::string& title) {
    bool next = false;
    SDL_Event e;
    std::cout << "Affichage : " << title << " (Fermez pour continuer)" << std::endl;

    while (!next) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) next = true;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Tracé de la première courbe
        graphique.draw_curve(S, v1, col1);
        // Tracé de la seconde courbe si demandé (superposition)
        if (double_plot) graphique.draw_curve(S, v2, col2);

        graphique.show();
        SDL_Delay(16);
    }
}

int main() {
    // 1. PARAMÈTRES (Identiques à votre consigne)
    const double K = 100.0, Smax = 300.0, sigma = 0.1, r = 0.1, T = 1.0;
    const int N = 1000, M = 1000;

    std::vector<double> S(M + 1);
    for (int i = 0; i <= M; ++i) S[i] = i * (Smax / M);

    // 2. CALCULS
    Option* call_opt = new Call(K, Smax, r, T);
    Option* put_opt  = new Put(K, Smax, r, T);

    // --- PUT ---
    EDP_complete edp_p_c(put_opt, sigma, r, T, Smax);
    EDP_reduite  edp_p_r(put_opt, sigma, r, T, Smax);
    
    cranck_nicolson sol_p_c(edp_p_c, N, M);
    sol_p_c.solve();
    std::vector<double> p_comp = sol_p_c.get_results()[0];

    implicite_solver sol_p_r(edp_p_r, N, M);
    sol_p_r.solve();
    std::vector<double> p_red = sol_p_r.get_results()[0];
    std::vector<double> p_err = compute_error(p_comp, p_red);

    // --- CALL ---
    EDP_complete edp_c_c(call_opt, sigma, r, T, Smax);
    EDP_reduite  edp_c_r(call_opt, sigma, r, T, Smax);

    cranck_nicolson sol_c_c(edp_c_c, N, M);
    sol_c_c.solve();
    std::vector<double> c_comp = sol_c_c.get_results()[0];

    implicite_solver sol_c_r(edp_c_r, N, M);
    sol_c_r.solve();
    std::vector<double> c_red = sol_c_r.get_results()[0];
    std::vector<double> c_err = compute_error(c_comp, c_red);

    // 3. INITIALISATION SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    SDL_Window* window = init_window("L3 Finance - Black Scholes", 800, 600);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    Sdl graphique(renderer, window);

    // Couleurs
    std::vector<Uint8> vert;  vert.push_back(0);   vert.push_back(255); vert.push_back(0);
    std::vector<Uint8> bleu;  bleu.push_back(0);   bleu.push_back(0);   bleu.push_back(255);
    std::vector<Uint8> rouge; rouge.push_back(255); rouge.push_back(0);   rouge.push_back(0);

    // 4. AFFICHAGE (Consigne 3.3)
    // 1. Fenêtre Put (Superposées)
    display_block(graphique, renderer, S, p_comp, vert, p_red, bleu, true, "Put : Complet (Vert) vs Reduit (Bleu)");
    
    // 2. Fenêtre Erreur Put
    display_block(graphique, renderer, S, p_err, rouge, p_err, rouge, false, "Erreur Put (Rouge)");

    // 3. Fenêtre Call (Superposées)
    display_block(graphique, renderer, S, c_comp, vert, c_red, bleu, true, "Call : Complet (Vert) vs Reduit (Bleu)");

    // 4. Fenêtre Erreur Call
    display_block(graphique, renderer, S, c_err, rouge, c_err, rouge, false, "Erreur Call (Rouge)");

    // NETTOYAGE
    cleanup(renderer, window);
    SDL_Quit();
    delete call_opt; 
    delete put_opt;

    return 0;
}
>>>>>>> Stashed changes
