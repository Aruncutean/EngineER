//
// Created by arunc on 27/06/2025.
//

#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <SDL3/SDL.h>

class GameWindow {

  public:
    GameWindow() = default;
     ~GameWindow() = default;

    void Init();
    void Resize(int width, int height);
    void Run();


private:
    SDL_Window* window = nullptr;
    bool isRunning = true;
    int width = 800;
    int height = 600;

    SDL_GLContext glContext = nullptr;
};


#endif //GAMEWINDOW_HPP
