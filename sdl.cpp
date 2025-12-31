/**
 * @file sdl.cpp
 * @brief Implémentation de la classe graphique utilisant SDL2.
 */
#include "sdl.hpp"
#include <iostream>

Sdl::Sdl(const char* title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erreur SDL: " << SDL_GetError() << std::endl;
    }
    win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
}

Sdl::~Sdl() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void Sdl::clear() {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
}

/**
 * @brief Affiche la courbe des prix. 
 * L'axe Y est inversé pour correspondre au repère SDL (0 en haut).
 */
void Sdl::plot(const std::vector<double>& data, SDL_Color color) {
    SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 255);
    for(size_t i = 0; i < data.size() - 1; ++i) {
        // Mise à l'échelle simple : 1 pixel par unité de S, hauteur 600
        SDL_RenderDrawLine(ren, (int)i, 550 - (int)data[i], (int)i+1, 550 - (int)data[i+1]);
    }
}

void Sdl::present() {
    SDL_RenderPresent(ren);
}