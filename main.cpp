/**
 * @file main.cpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Implémentation du programme principal compatible C++98
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> // Pour std::abs
#include "payoff.hpp"
#include "edp.hpp"
#include "solver.hpp"
#include "sdl.hpp"

int main() {
    std::cout << "Affichage des courbes :" << std::endl;
    std::cout << "  Appuyer sur 'C' pour afficher le CALL" << std::endl;
    std::cout << "  Appuyer sur 'P' pour afficher le PUT" << std::endl;
    std::cout << "  Appuyer sur ESPACE pour basculer entre PRIX et ERREUR" << std::endl;

    double K = 100.0; 
    double L = 300.0;
    double sigma = 0.1;
    double r = 0.1; 
    double T = 1.0;
    int N = 1000; 
    int M = 1000;

    // Grille de référence pour l'affichage (linéaire)
    std::vector<double> s(N + 1);
    for (int i = 0; i <= N; ++i) s[i] = i * (L / N);

    // CALCULS POUR LE CALL
    Option* call = new Call(K, L, r, T);
    EDP edp_call(call, sigma, r, T, L);
    
    // Solveur Complet (Crank-Nicolson)
    Cranck_nicolson solver_c_call(edp_call, N, M);
    solver_c_call.solve();
    // CN : t=0 est à l'indice 0
    std::vector<double> res_call_comp = solver_c_call.get_results()[0];

    // Solveur Réduit (Méthode implicite sur équation de chaleur avec changement de variable)
    Implicite_solver solver_r_call(edp_call, N, M);
    solver_r_call.solve(); // on effectue le changement de variable inverse après avoir trouvé la solution

    // Interpolation linéaire : on aligne le réduit sur la grille 's'
    std::vector<double> res_call_red_aligned(N + 1);
    for (int i = 0; i <= N; ++i) {
        res_call_red_aligned[i] = solver_r_call.get_value_at_S(s[i], 0);
    }

    // CALCULS POUR LE PUT
    Option* put = new Put(K, L, r, T); 
    EDP edp_put(put, sigma, r, T, L);
    
    // Solveur Complet (Crank-Nicolson)
    Cranck_nicolson solver_c_put(edp_put, N, M);
    solver_c_put.solve();
    std::vector<double> res_put_comp = solver_c_put.get_results()[0];

    // Solveur Réduit (Méthode implicite sur équation de chaleur avec changement de variable)
    Implicite_solver solver_r_put(edp_put, N, M);
    solver_r_put.solve();

    // Interpolation linéaire : on aligne le réduit sur la grille 's'
    std::vector<double> res_put_red_aligned(N + 1);
    for (int i = 0; i <= N; ++i) {
        res_put_red_aligned[i] = solver_r_put.get_value_at_S(s[i], 0);
    }

    // calcul des erreurs 
    std::vector<double> err_call(N + 1), err_put(N + 1);
    for (int i = 0; i <= N; ++i) {
        err_call[i] = std::abs(res_call_comp[i] - res_call_red_aligned[i]);
        err_put[i] = std::abs(res_put_comp[i] - res_put_red_aligned[i]);
    }

    // affichage avec SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    SDL_Window* window = init_window("Analyse Black-Scholes", 640, 480);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    Sdl graphique(renderer, window);

    std::vector<Uint8> colorGreen(3); colorGreen[0]=0; colorGreen[1]=255; colorGreen[2]=0;
    std::vector<Uint8> colorCyan(3);  colorCyan[0]=0;  colorCyan[1]=255;  colorCyan[2]=255;
    std::vector<Uint8> colorYellow(3);colorYellow[0]=255; colorYellow[1]=255; colorYellow[2]=0;

    bool quit = false; SDL_Event e; int option_type = 1; int view_mode = 1;

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

        if (option_type == 1) {
            if (view_mode == 1) {
                graphique.draw_curve(s, res_call_comp, colorGreen);
                graphique.draw_curve(s, res_call_red_aligned,  colorCyan);
            } else graphique.draw_curve(s, err_call, colorYellow);
        } else {
            if (view_mode == 1) {
                graphique.draw_curve(s, res_put_comp, colorGreen);
                graphique.draw_curve(s, res_put_red_aligned,  colorCyan);
            } else graphique.draw_curve(s, err_put, colorYellow);
        }
        graphique.show();
        SDL_Delay(16);
    }

    cleanup(renderer, window); SDL_Quit();
    delete call; delete put; 
    return 0;
}