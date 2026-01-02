/**
 * @file main.cpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Implémentation du programme principal pour la résolution de l'équation de Black-Scholes
 */

#include <iostream>
#include <vector>
#include <cmath>
#include "payoff.hpp"
#include "edp.hpp"
#include "solver.hpp"
#include "sdl.hpp"

/**
 * @brief Fonction principale
 * Cette fonction initialise les paramètres financiers, résout l'équation de Black-Scholes
 * Elle affiche ensuite les résultats 
 * @return 0 si le programme s'est exécuté correctement
 */

int main() {
    /*paramètre financiers*/
    double K = 100.0;    // strike 
    double L = 300.0;    // prix maximal de l'actif
    double sigma = 0.1;  // Volatilité de l'actif
    double r = 0.1;      //taux d'intéret du marché
    double T = 1.0;      // temps terminal 
    int N = 1000;        // intervalles de discrétisation en espace
    int M = 1000;        // intervalles de discrétisation en temps

    std::vector<double> s(N + 1);       //prix de l'actif 
    for (int i = 0; i <= N; ++i) {
        s[i] = i * (L / N);     
    }

    //On calcule pour le CALL
    Option* call = new Call(K, L, r, T);
    //En premier avec l'EDP complète et la méthode Cranck-Nicolson
    EDP_complete edp_c_call(call, sigma, r, T, L);
    Cranck_nicolson solver_c_call(edp_c_call, N, M);
    solver_c_call.solve();
    std::vector<double> res_call_comp = solver_c_call.get_results()[0];
    //Ensuite avec l'EDP réduite et la méthode Implicite
    EDP_reduite edp_r_call(call, sigma, r, T, L);
    Implicite_solver solver_r_call(edp_r_call, N, M);
    solver_r_call.solve();
    std::vector<double> res_call_red = solver_r_call.get_results()[0];

    //On calcule pour le PUT
    Option* put = new Put(K, L, r, T); // Assure-toi d'avoir la classe Put
    //Avec l'EDP complète et la méthode Cranck-Nicolson
    EDP_complete edp_c_put(put, sigma, r, T, L);
    Cranck_nicolson solver_c_put(edp_c_put, N, M);
    solver_c_put.solve();
    std::vector<double> res_put_comp = solver_c_put.get_results()[0];
    //Puis avec l'EDP réduite et la méthode Implicite
    EDP_reduite edp_r_put(put, sigma, r, T, L);
    Implicite_solver solver_r_put(edp_r_put, N, M);
    solver_r_put.solve();
    std::vector<double> res_put_red = solver_r_put.get_results()[0];

    //Calcul des erreurs entre les deux méthodes pour le CALL et le PUT
    std::vector<double> err_call(N + 1), err_put(N + 1);
    for (int i = 0; i <= N; ++i) {
        err_call[i] = std::abs(res_call_comp[i] - res_call_red[i]);
        err_put[i] = std::abs(res_put_comp[i] - res_put_red[i]);
    }

    //on initialise la fenêtre SDL pour l'affichage
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    SDL_Window* window = init_window("Analyse Black-Scholes", 640, 480);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    Sdl graphique(renderer, window);

    bool quit = false;
    SDL_Event e;
    int option_type = 1; // 1 pour le CALL, 2 pour le PUT
    int view_mode = 1;   // 1 pour les méthodes superposées, 2 pour l'erreur
    //commandes claviers
    std::cout << "\n Commandes claviers :" << std::endl;
    std::cout << "C  : Afficher le CALL" << std::endl;
    std::cout << "P  : Afficher le PUT" << std::endl;
    std::cout << "Espace  : Basculer entre méthodes / Erreur" << std::endl;

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
                graphique.draw_curve(s, res_call_comp, {0, 255, 0}); //verte = EDP complète
                graphique.draw_curve(s, res_call_red,  {0, 255, 255}); //Cyan = EDP réduite
            } else {
                graphique.draw_curve(s, err_call, {255, 255, 0}); //Jaune = Erreur
            }
        } else {
            if (view_mode == 1) {
                graphique.draw_curve(s, res_put_comp, {0, 255, 0});   // Vert : Complet
                graphique.draw_curve(s, res_put_red,  {0, 255, 255}); // Cyan : Réduit
            } else {
                graphique.draw_curve(s, err_put, {255, 255, 0});    // Jaune : Erreur
            }
        }

        graphique.show();
        SDL_Delay(16);
    }

    cleanup(renderer, window);
    SDL_Quit();
    delete call; 
    delete put;
    return 0;
}