//
// Created by arunc on 27/06/2025.
//

#include "GameWindow.hpp"

#include <cstdio>
#include <glad/glad.h>
#include <SDL3/SDL.h>

void GameWindow::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        isRunning = false;
        return;
    }

#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
 SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
    window = SDL_CreateWindow("Game Window", width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        isRunning = false;
        return;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        SDL_Log("Failed to initialize glContext (OpenGL desktop)");
        isRunning = false;
        return;
    }

    // Inițializează GLAD
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD (OpenGL desktop)");
        isRunning = false;
        return;
    }
}

void GameWindow::Resize(int w, int h) {
    width = w;
    height = h;
}

void GameWindow::Run() {
    if (!isRunning) return;

    SDL_Event event;
    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        SDL_GL_SwapWindow(window);
    }


    SDL_DestroyWindow(window);
    SDL_Quit();
}
