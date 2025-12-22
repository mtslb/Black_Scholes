#include "sdl.hpp"

sdl::sdl(const std::string& title, const position& pos, const size& sz) {
    window = SDL_CreateWindow(title.c_str(), pos.x, pos.y, sz.width, sz.height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        std::cout << "Erreur lors de la création de la fenêtre : " << SDL_GetError() << std::endl;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        std::cout << "Erreur lors de la création du renderer : " << SDL_GetError() << std::endl;
    }
}

sdl::~sdl() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

bool sdl::is_running = false;

void sdl::init() {
    SDL_Init(SDL_INIT_VIDEO);
    is_running = true;
}

void sdl::exit() {
    SDL_Quit();
}

void sdl::exit_run() {
    is_running = false;
}

void sdl::run() {
    SDL_Event e;
    while (is_running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                exit_run();
            }
        }
    }
}

void sdl::show() {
    SDL_ShowWindow(window);
}

size sdl::get_size() const {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return size(w, h);
}

position sdl::get_position() const {
    int x, y;
    SDL_GetWindowPosition(window, &x, &y);
    return position(x, y);
}

void sdl::move(const position& new_pos) {
    SDL_SetWindowPosition(window, new_pos.x, new_pos.y);
}

void sdl::resize(const size& new_size) {
    SDL_SetWindowSize(window, new_size.width, new_size.height);
}


SDL_Renderer* sdl::get_renderer() const {
    return renderer;
}

void draw_curve(SDL_Renderer* renderer, const std::vector<float>& values, int width, int height) {
    if (values.empty()) return;

    // Effacer le renderer (fond noir)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Couleur de la courbe (rouge)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Échelle pour adapter la courbe à la fenêtre
    float max_val = *std::max_element(values.begin(), values.end());
    float min_val = *std::min_element(values.begin(), values.end());
    float range = max_val - min_val;
    if (range == 0) range = 1;

    int n = values.size();
    for (int i = 0; i < n - 1; ++i) {
        int x1 = i * width / (n - 1);
        int y1 = height - (int)((values[i] - min_val) / range * height);
        int x2 = (i + 1) * width / (n - 1);
        int y2 = height - (int)((values[i + 1] - min_val) / range * height);

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    // Afficher le renderer
    SDL_RenderPresent(renderer);
}
