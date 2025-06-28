 #include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "GameWindow.hpp"


int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    GameWindow window;
    window.Init();

    window.Run();

    return 0;
}
