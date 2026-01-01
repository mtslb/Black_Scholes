#include <iostream>
#include <vector>
#include <cmath>
#include "payoff.hpp"
#include "edp.hpp"
#include "solver.hpp"
#include "sdl.hpp"

int main() {
    // --- 1. CONFIGURATION (M=1000, N=1000) ---
    double K = 100.0, Smax = 300.0, sigma = 0.2, r = 0.05, T = 1.0;
    int N = 1000, M = 1000;

    std::vector<double> S(N + 1);
    for (int i = 0; i <= N; ++i) S[i] = i * (Smax / N);

    // --- 2. CALCULS POUR LE CALL ---
    std::cout << "Calculs Call en cours..." << std::endl;
    Option* call = new Call(K, Smax, r, T);
    
    EDP_complete edp_c_call(call, sigma, r, T, Smax);
    cranck_nicolson solver_c_call(edp_c_call, N, M);
    solver_c_call.solve();
    std::vector<double> res_call_comp = solver_c_call.get_results()[0];

    EDP_reduite edp_r_call(call, sigma, r, T, Smax);
    implicite_solver solver_r_call(edp_r_call, N, M);
    solver_r_call.solve();
    std::vector<double> res_call_red = solver_r_call.get_results()[0];

    // --- 3. CALCULS POUR LE PUT ---
    std::cout << "Calculs Put en cours..." << std::endl;
    Option* put = new Put(K, Smax, r, T); // Assure-toi d'avoir la classe Put
    
    EDP_complete edp_c_put(put, sigma, r, T, Smax);
    cranck_nicolson solver_c_put(edp_c_put, N, M);
    solver_c_put.solve();
    std::vector<double> res_put_comp = solver_c_put.get_results()[0];

    EDP_reduite edp_r_put(put, sigma, r, T, Smax);
    implicite_solver solver_r_put(edp_r_put, N, M);
    solver_r_put.solve();
    std::vector<double> res_put_red = solver_r_put.get_results()[0];

    // --- 4. CALCUL DES ERREURS ---
    std::vector<double> err_call(N + 1), err_put(N + 1);
    for (int i = 0; i <= N; ++i) {
        err_call[i] = std::abs(res_call_comp[i] - res_call_red[i]);
        err_put[i] = std::abs(res_put_comp[i] - res_put_red[i]);
    }

    // --- 5. AFFICHAGE SDL ---
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    SDL_Window* window = init_window("Analyse Black-Scholes", 640, 480);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    Sdl graphique(renderer, window);

    bool quit = false;
    SDL_Event e;
    int option_type = 1; // 1: Call, 2: Put
    int view_mode = 1;   // 1: Superposition, 2: Erreur

    std::cout << "\n--- COMMANDES CLAVIER ---" << std::endl;
    std::cout << "[C] : Afficher le CALL" << std::endl;
    std::cout << "[P] : Afficher le PUT" << std::endl;
    std::cout << "[ESPACE] : Basculer entre Superposition / Erreur" << std::endl;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_c) option_type = 1;
                if (e.key.keysym.sym == SDLK_p) option_type = 2;
                if (e.key.keysym.sym == SDLK_SPACE) view_mode = (view_mode == 1) ? 2 : 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (option_type == 1) { // MODE CALL
            if (view_mode == 1) {
                graphique.draw_curve(S, res_call_comp, {0, 255, 0});   // Vert : Complet
                graphique.draw_curve(S, res_call_red,  {0, 255, 255}); // Cyan : Réduit
            } else {
                graphique.draw_curve(S, err_call, {255, 255, 0});      // Jaune : Erreur
            }
        } else { // MODE PUT
            if (view_mode == 1) {
                graphique.draw_curve(S, res_put_comp, {255, 0, 0});   // Rouge : Complet
                graphique.draw_curve(S, res_put_red,  {255, 165, 0}); // Orange : Réduit
            } else {
                graphique.draw_curve(S, err_put, {255, 255, 0});       // Jaune : Erreur
            }
        }

        graphique.show();
        SDL_Delay(16);
    }

    cleanup(renderer, window);
    SDL_Quit();
    delete call; delete put;
    return 0;
}
