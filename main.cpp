/**
 * @file main.cpp
 * @brief Résolution des EDP de Black-Scholes avec affichage stabilisé.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> // Pour std::min
#include "payoff.hpp"
#include "solver.hpp"
#include "sdl.hpp"

/**
 * @brief Calcule l'écart absolu avec un facteur d'échelle raisonnable.
 */
std::vector<double> compute_divergence(const std::vector<double>& v1, const std::vector<double>& v2) {
    std::vector<double> res(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        // On réduit le facteur à 100.0 pour éviter que ça ne sorte de l'écran
        // L'erreur est souvent forte là où S est grand ou proche de K.
        res[i] = std::abs(v1[i] - v2[i]) * 100.0; 
    }
    return res;
}

int main() {
    // Valeurs numériques imposées
    const double T = 1.0;
    const double r = 0.1;
    const double sigma = 0.1;
    const double K = 100.0;
    const double L = 300.0;
    const int M = 1000;
    const int N = 1000;

    BSSolver engine(r, sigma, T, L, K);

    // --- CALCULS ---
    PutPayoff put_pay(K);
    std::vector<double> p_comp = engine.solve_complete(put_pay, M, N);
    std::vector<double> p_red  = engine.solve_reduced(put_pay, M, N);
    std::vector<double> p_err  = compute_divergence(p_comp, p_red);

    CallPayoff call_pay(K);
    std::vector<double> c_comp = engine.solve_complete(call_pay, M, N);
    std::vector<double> c_red  = engine.solve_reduced(call_pay, M, N);
    std::vector<double> c_err  = compute_divergence(c_comp, c_red);

    // --- AFFICHAGE SDL ---
    // Augmentation de la taille pour voir l'ensemble du domaine L=300
    const int W = 1200; 
    const int H = 800;

    Sdl win_put("Put: CN (Vert) vs Implicite (Bleu)", W, H);
    Sdl win_err_p("Erreur Put (x100)", W, H);
    Sdl win_call("Call: CN (Vert) vs Implicite (Bleu)", W, H);
    Sdl win_err_c("Erreur Call (x100)", W, H);

    SDL_Color green = {0, 255, 0, 255};
    SDL_Color blue  = {0, 0, 255, 255};
    SDL_Color red   = {255, 0, 0, 255};

    bool is_active = true;
    SDL_Event ev;

    while (is_active) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) is_active = false;
        }

        // Rendu des fenêtres
        win_put.clear();
        win_put.plot(p_comp, green); win_put.plot(p_red, blue);
        win_put.present();

        win_err_p.clear();
        win_err_p.plot(p_err, red);
        win_err_p.present();

        win_call.clear();
        win_call.plot(c_comp, green); win_call.plot(c_red, blue);
        win_call.present();

        win_err_c.clear();
        win_err_c.plot(c_err, red);
        win_err_c.present();
    }

    SDL_Quit();
    return 0;
}