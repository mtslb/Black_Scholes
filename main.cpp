#include <iostream>
#include <vector>
#include "payoff.hpp"
#include "edp.hpp"
#include "solver.hpp"
#include "sdl.hpp"

int main() {
    // 1. PARAMÈTRES COMMUNS
    double K = 100.0, Smax = 300.0, sigma = 0.2, r = 0.05, T = 1.0;
    int N = 200, M = 200;

    // 2. GÉNÉRATION DE L'AXE S (Indispensable pour draw_curve)
    std::vector<double> S(N + 1);
    double dS = Smax / N;
    for (int i = 0; i <= N; ++i) S[i] = i * dS;

    // 3. CALCULS DES 4 CAS
    std::cout << "Calcul des 4 scenarios en cours..." << std::endl;

    // Création des Options
    Option* call = new Call(K, Smax, r, T);
    Option* put  = new Put(K, Smax, r, T);

    // EDP_complete
    EDP_complete edp_c_call(call, sigma, r, T, Smax);
    EDP_complete edp_c_put(put, sigma, r, T, Smax);

    // EDP_reduite (Assure-toi que cette classe existe dans edp.hpp)
    EDP_reduite edp_r_call(call, sigma, r, T, Smax);
    EDP_reduite edp_r_put(put, sigma, r, T, Smax);

    // Solvers pour Call
    cranck_nicolson solver1(edp_c_call, N, M);
    solver1.solve();
    std::vector<double> res_call_comp = solver1.get_results()[0];

    implicite_solver solver2(edp_r_call, N, M);
    solver2.solve();
    std::vector<double> res_call_red = solver2.get_results()[0];

    // Solvers pour Put
    cranck_nicolson solver3(edp_c_put, N, M);
    solver3.solve();
    std::vector<double> res_put_comp = solver3.get_results()[0];

    implicite_solver solver4(edp_r_put, N, M);
    solver4.solve();
    std::vector<double> res_put_red = solver4.get_results()[0];

    std::cout << "Tous les calculs sont finis." << std::endl;

    // 4. INITIALISATION SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    SDL_Window* window = init_window("Black-Scholes : 4 Scenarios", 640, 480);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    // ON DÉCLARE GRAPHIQUE ICI (Après le renderer)
    Sdl graphique(renderer, window);
    
    bool quit = false;
    SDL_Event e;
    int mode = 1; // 1: Call Comp, 2: Call Red, 3: Put Comp, 4: Put Red

    std::cout << "Appuyez sur 1, 2, 3 ou 4 pour changer de graphique" << std::endl;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_1) mode = 1;
                if (e.key.keysym.sym == SDLK_2) mode = 2;
                if (e.key.keysym.sym == SDLK_3) mode = 3;
                if (e.key.keysym.sym == SDLK_4) mode = 4;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 5. AFFICHAGE SELON LE MODE CHOISI
        if (mode == 1) graphique.draw_curve(S, res_call_comp, {0, 255, 0});   // Vert
        if (mode == 2) graphique.draw_curve(S, res_call_red,  {0, 255, 255}); // Cyan
        if (mode == 3) graphique.draw_curve(S, res_put_comp,  {255, 0, 0});   // Rouge
        if (mode == 4) graphique.draw_curve(S, res_put_red,   {255, 165, 0}); // Orange

        graphique.show();
        SDL_Delay(16);
    }

    // Nettoyage
    cleanup(renderer, window);
    SDL_Quit();
    delete call; delete put;
    return 0;
}
