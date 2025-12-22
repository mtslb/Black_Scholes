#ifndef SDL_CORE_HPP
#define SDL_CORE_HPP

#include <string>
#include <iostream>
#include <SDL2/SDL.h>

struct position {
    int x;
    int y;
    // Constructeur pratique
    position(int xpos = 100, int ypos = 100) : x(xpos), y(ypos) {}

};
typedef struct position pos;

struct Size {
    int width;
    int height;

    // Constructeur pratique
    Size(int w = 800, int h = 600) : width(w), height(h) {}
};
typedef struct Size size;

class sdl {
    private:
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    static bool is_running;

    public:
    
    sdl(const std::string& title, const position& pos, const size& sz);
    ~sdl();
    
    static void init();
    static void exit();
    static void run();
    static void exit_run();
    void show();
    size get_size() const;
    position get_position() const;
    void move(const position& new_pos);
    void resize(const size& new_size);
    SDL_Renderer* get_renderer() const;  
    void draw_curve(SDL_Renderer* renderer, const std::vector<float>& values);
};

#endif // SDL_CORE_HPP