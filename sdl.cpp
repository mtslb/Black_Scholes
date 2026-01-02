/**
 * @file sdl.cpp
 * @author Mathias LE BOUEDEC - Lilou MALFOY
 * @date 2025
 * @brief Implémentation de la classe Sdl
 */

#include "sdl.hpp" 
 
/**
 * @brief Initialise la fenêtre SDL
 * @param title Titre de la fenêtre
 * @param width Largeur de la fenêtre en pixels
 * @param height Hauteur de la fenêtre en pixels
 * @return Pointeur vers la fenêtre SDL créée, ou nullptr en cas d'erreur
 */
SDL_Window* init_window(const std::string& title, int width, int height)
{
    SDL_Window* window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        std::cout << "Erreur lors de la création de la fenêtre : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return window;
}

/**
 * @brief Initialise le renderer SDL
 * @param window Fenêtre SDL à utiliser pour le renderer
 * @return Pointeur vers le renderer SDL créé, ou nullptr en cas d'erreur
 */
SDL_Renderer* init_renderer(SDL_Window* window) {

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    
    if (renderer == nullptr) {
        std::cout << "Erreur Renderer : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return renderer;
}

/**
* @brief Détruit le renderer et le window
*/
void cleanup(SDL_Renderer* renderer, SDL_Window* window)
{
    //détruire le renderer et le window 
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

/**
 * @brief Constructeur par défaut
 */
Sdl::Sdl() : renderer_(nullptr), window_(nullptr) {} // Initialisation de renderer_ et window_ avec nullptr

/**
 * @brief Constructeur
 * @param renderer Renderer SDL à utiliser pour dessiner les courbes
 * @param window Window SDL à utiliser pour dessiner les courbes
 */
Sdl::Sdl(SDL_Renderer* renderer, SDL_Window* window) : renderer_(renderer), window_(window) {}

/**
 * @brief Destructeur
 */
Sdl::~Sdl(){
    cleanup(renderer_, window_);
}

/**
 * @brief Affiche une courbe dans la fenêtre
 * @param x Vecteur correspondant aux abscisses
 * @param y Vecteur correspondant aux odronnées
 * @param color Couleur de la courbe, sous la forme d'un vecteur de trois entiers non signés de 8 bits
 */
void Sdl::draw_curve(const std::vector<double>& x, const std::vector<double>& y, const std::vector<Uint8>& color)
{
    if (x.empty() || y.empty()) return;

    int window_width = 640;
    int window_height = 480;
    int margin = 30;

    //trouver les valeurs min et max pour le redimensionnement
    double xmin = *std::min_element(x.begin(), x.end());
    double xmax = *std::max_element(x.begin(), x.end());
    double ymin = *std::min_element(y.begin(), y.end());
    double ymax = *std::max_element(y.begin(), y.end());

    //evite la division par zéro
    double dy = ymax - ymin;
    if (dy < 1e-6) dy = 1.0;
    double dx = xmax - xmin;
    if (dx < 1e-6) dx = 1.0;

    const double xscale = static_cast<double>(window_width - 2 * margin) / dx;
    const double yscale = static_cast<double>(window_height - 2 * margin) / dy;

    SDL_SetRenderDrawColor(renderer_, color[0], color[1], color[2], 255);
    for (std::size_t i = 1; i < x.size(); i++)
    {
        int x1 = static_cast<int>(margin + (x[i - 1] - xmin) * xscale);
        int y1 = static_cast<int>(window_height - margin - (y[i - 1] - ymin) * yscale);
        int x2 = static_cast<int>(margin + (x[i] - xmin) * xscale);
        int y2 = static_cast<int>(window_height - margin - (y[i] - ymin) * yscale);

        SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
    }
}

/**
 * @brief affiche la fenêtre
 */
void Sdl::show()
{
    SDL_RenderPresent(renderer_);
}