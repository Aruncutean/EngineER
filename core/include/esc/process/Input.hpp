//
// Created by arunc on 01/07/2025.
//

#ifndef INPUT_HPP
#define INPUT_HPP

#include <SDL3/SDL.h>
#include "glm.hpp"
#include <functional>
#include <unordered_map>

class Input {
public:
    using Key = SDL_Keycode;
    enum class MouseButton : Uint8 {
        Left = SDL_BUTTON_LEFT,
        Right = SDL_BUTTON_RIGHT,
        Middle = SDL_BUTTON_MIDDLE
    };

    static bool isActive;
    static Input& Instance();

    static std::vector < std::function<void(Key)>> KeyPressed;
    static std::vector < std::function<void(Key)>> KeyReleased;
    static std::vector < std::function<void(MouseButton)>> MouseClicked;
    static std::vector < std::function<void(MouseButton)>> MouseReleased;
    static std::vector < std::function<void(glm::vec2)>> MouseMove;

    static void ProcessEvent(const SDL_Event& e);
    static void ResetScroll();
    static bool IsKeyDown(Key key);
    static bool IsMouseButtonDown(MouseButton btn);
    static glm::vec2 GetMousePosition();
    static glm::vec2 GetScrollDelta();

    static void MouseMoveCall(glm::vec2);

    static bool KeyW();
    static bool KeyS();
    static bool KeyA();
    static bool KeyD();
    static bool KeyR();
    static bool KeyT();



private:
    Input() = default;

    static std::unordered_map<Key, bool> keys;
    static std::unordered_map<Uint8, bool> mouseButtons;
    static glm::vec2 mousePosition;
    static glm::vec2 scrollDelta;
};

#endif //INPUT_HPP
