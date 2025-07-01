//
// Created by arunc on 01/07/2025.
//

#include "esc/process/Input.hpp"

std::vector < std::function<void(Input::Key)>> Input::KeyPressed;
std::vector < std::function<void(Input::Key)>> Input::KeyReleased;
std::vector < std::function<void(Input::MouseButton)>> Input::MouseClicked;
std::vector < std::function<void(Input::MouseButton)>> Input::MouseReleased;
std::vector < std::function<void(glm::vec2)>> Input::MouseMove;

std::unordered_map<Input::Key, bool> Input::keys;
std::unordered_map<Uint8, bool> Input::mouseButtons;
glm::vec2 Input::mousePosition{ 0.0f };
glm::vec2 Input::scrollDelta{ 0.0f };
bool Input::isActive = false;

Input& Input::Instance() {
	static Input instance;
	return instance;
}

void Input::ProcessEvent(const SDL_Event& e) {

	//if (e.type == SDL_MOUSEMOTION)
	//{
	//	mousePosition = glm::vec2(e.motion.x, e.motion.y);
	//	for (auto& cb : MouseMove)
	//		cb(mousePosition);
	//}
	if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
		mouseButtons[e.button.button] = false;
		for (auto& cb : MouseReleased)
			cb(static_cast<MouseButton>(e.button.button));
	}

	if (isActive == true) {
		switch (e.type) {
		case SDL_EVENT_KEY_DOWN:
			if (!e.key.repeat) {
				keys[e.key.key] = true;
				for (auto& cb : KeyPressed)
					cb(e.key.key);
			}
			break;
		case SDL_EVENT_KEY_UP:
			keys[e.key.key] = false;
			for (auto& cb : KeyReleased)
				cb(e.key.key);
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			mouseButtons[e.button.button] = true;
			for (auto& cb : MouseClicked)
				cb(static_cast<MouseButton>(e.button.button));
			break;



		case SDL_EVENT_MOUSE_WHEEL:
			scrollDelta = glm::vec2(static_cast<float>(e.wheel.x), static_cast<float>(e.wheel.y));
			break;
		}
	}
}

void Input::MouseMoveCall(glm::vec2 mousePosition) {
	for (auto& cb : MouseMove)
		cb(mousePosition);
}

void Input::ResetScroll() {
	scrollDelta = glm::vec2(0.0f);
}

bool Input::IsKeyDown(Key key) {
	auto it = keys.find(key);
	return it != keys.end() && it->second;
}

bool Input::IsMouseButtonDown(MouseButton btn) {
	auto it = mouseButtons.find(static_cast<Uint8>(btn));
	return it != mouseButtons.end() && it->second;
}

glm::vec2 Input::GetMousePosition() {
	return mousePosition;
}

glm::vec2 Input::GetScrollDelta() {
	return scrollDelta;
}

bool Input::KeyW() { return IsKeyDown(SDLK_W); }
bool Input::KeyS() { return IsKeyDown(SDLK_S); }
bool Input::KeyA() { return IsKeyDown(SDLK_A); }
bool Input::KeyD() { return IsKeyDown(SDLK_D); }
bool Input::KeyT() { return IsKeyDown(SDLK_T); }
bool Input::KeyR() { return IsKeyDown(SDLK_R); }