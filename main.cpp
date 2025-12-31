/**
 * @file main.cpp
 * @brief Programme principal de résolution des EDP de Black-Scholes.
 * * Ce fichier orchestre la résolution par Crank-Nicolson et schéma implicite,
 * puis affiche les courbes de prix et d'erreur via la classe Sdl.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "payoff.hpp"
#include "solver.hpp"
#include "sdl.hpp"

/**
 * @brief Calcule la différence absolue entre deux vecteurs de résultats.
 * @param v1 Premier vecteur (Crank-Nicolson).
 * @param v2 Second vecteur (Schéma implicite).
 * @return std::vector<double> Vecteur de l'erreur absolue.
 */
std::vector<double> get_error(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> res(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        res[i] = std::abs(v1[i] - v2[i]) * 100.0; // Facteur 100 pour la visibilité de l'erreur
    }
    return res;
}

/**
 * @brief Point d'entrée du programme.
 * @return int Code de sortie.
 */
int main() {
    // --- Configuration des paramètres (Source [cite: 72-76]) ---
    const double T = 1.0;
    const double r = 0.1;
    const double sigma = 0.1;
    const double K = 100.0;
    const double L = 300.0;
    
    // Discrétisation (Source )
    const int M = 1000;
    const int N = 1000;

    // --- Moteurs de calcul ---
    BSSolver engine(r, sigma, T, L, K);
    CallPayoff call_pay(K);
    PutPayoff put_pay(K);

    // --- Calcul des solutions C(0, s) ---
    // Pour le Call
    std::vector<double> call_comp = engine.solve_complete(call_pay, M, N);
    std::vector<double> call_red  = engine.solve_reduced(call_pay, M, N);
    std::vector<double> call_err  = get_error(call_comp, call_red);

    // Pour le Put
    std::vector<double> put_comp = engine.solve_complete(put_pay, M, N);
    std::vector<double> put_red  = engine.solve_reduced(put_pay, M, N);
    std::vector<double> put_err  = get_error(put_comp, put_red);

    // --- Initialisation des fenêtres SDL (Source [cite: 87, 88]) ---
    Sdl sdl_call("Call: Crank-Nicolson (Vert) vs Implicite (Bleu)", 800, 600);
    Sdl sdl_err_call("Erreur Call (x100)", 800, 600);
    
    Sdl sdl_put("Put: Crank-Nicolson (Vert) vs Implicite (Bleu)", 800, 600);
    Sdl sdl_err_put("Erreur Put (x100)", 800, 600);

    // Couleurs SDL_Color
    SDL_Color green = {0, 255, 0, 255};
    SDL_Color blue  = {0, 0, 255, 255};
    SDL_Color red   = {255, 0, 0, 255};

    // --- Boucle de rendu ---
    bool is_running = true;
    SDL_Event event;

    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) is_running = false;
        }

        // Affichage CALL
        sdl_call.clear();
        sdl_call.plot(call_comp, green); // Solution EDP complète
        sdl_call.plot(call_red, blue);   // Solution EDP réduite superposée
        sdl_call.present();

        sdl_err_call.clear();
        sdl_err_call.plot(call_err, red); // Fenêtre d'erreur
        sdl_err_call.present();

        // Affichage PUT
        sdl_put.clear();
        sdl_put.plot(put_comp, green);
        sdl_put.plot(put_red, blue);
        sdl_put.present();

        sdl_err_put.clear();
        sdl_err_put.plot(put_err, red);
        sdl_err_put.present();
    }

    return 0;
}