/**
 * @file Sdl.hpp
 * @brief Encapsulation de la bibliothèque SDL2 pour l'affichage des courbes.
 */
#ifndef SDL_HPP
#define SDL_HPP

#include <SDL2/SDL.h>
#include <vector>

/**
 * @class Sdl
 * @brief Classe facilitant l'usage de SDL2 pour tracer des graphiques.
 */
class Sdl {
    SDL_Window* win;
    SDL_Renderer* ren;
public:
    /**
     * @brief Initialise la fenêtre et le moteur de rendu.
     * @param title Titre de la fenêtre.
     * @param w Largeur.
     * @param h Hauteur.
     */
    Sdl(const char* title, int w, int h);

    /** @brief Destructeur libérant les ressources SDL2. */
    ~Sdl();

    /**
     * @brief Trace une courbe à partir d'un vecteur de données.
     * @param data Valeurs à afficher.
     * @param color Couleur SDL_Color pour le tracé.
     */
    void plot(const std::vector<double>& data, SDL_Color color);

    /** @brief Efface l'écran (fond noir). */
    void clear();

    /** @brief Met à jour l'affichage de la fenêtre. */
    void present();
};

#endif // SDL_HPP