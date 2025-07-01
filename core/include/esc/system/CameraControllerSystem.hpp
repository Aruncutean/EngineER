//
// Created by arunc on 01/07/2025.
//

#ifndef CAMERACONTROLLERSYSTEM_HPP
#define CAMERACONTROLLERSYSTEM_HPP
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <memory>
#include "esc/component/CameraComponent.hpp"
#include "esc/component/TransformComponent.hpp"
#include "esc/entity/Entity.hpp"
#include "esc/process/Input.hpp"

namespace System {

	class CameraControllerSystem : public std::enable_shared_from_this<CameraControllerSystem> {
	private:
		glm::vec2 lastMousePos{};
		bool firstMove = true;
		float yaw = -90.0f;
		float pitch = 0.0f;

		bool mousePressed = false;
		float moveSpeed = 35.0f;
		float lookSensitivity = 0.1f;

		std::shared_ptr<Component::CameraComponent> camera;
		std::shared_ptr<Component::TransformComponent> transform;

	public:
		explicit CameraControllerSystem(std::shared_ptr<Entity::Entity> cameraEntity) {
			transform = cameraEntity->GetComponent<Component::TransformComponent>();
			camera = cameraEntity->GetComponent<Component::CameraComponent>();
		}

		void InitCallbacks() {
			std::weak_ptr<CameraControllerSystem> weakSelf = shared_from_this();

			Input::Instance().MouseClicked.push_back([weakSelf](Input::MouseButton mouse) {
				if (auto self = weakSelf.lock()) {
					if (mouse == Input::MouseButton::Middle) {
						self->mousePressed = true;
					}
				}
				}
			);

			Input::Instance().MouseReleased.push_back([weakSelf](Input::MouseButton mouse) {
				if (auto self = weakSelf.lock()) {
					if (mouse == Input::MouseButton::Middle) {
						self->mousePressed = false;
					}
				}
				});

			Input::Instance().MouseMove.push_back([weakSelf](glm::vec2 pos) {
				if (auto self = weakSelf.lock()) {
					self->OnMouseMove(pos);

				}});

		}

		void OnMouseMove(const glm::vec2& newPos) {
			if (firstMove) {
				lastMousePos = newPos;
				firstMove = false;
			}

			float xoffset = newPos.x - lastMousePos.x;
			float yoffset = lastMousePos.y - newPos.y;
			lastMousePos = newPos;

			if (!mousePressed) return;

			xoffset *= lookSensitivity;
			yoffset *= lookSensitivity;

			yaw += xoffset;
			pitch += yoffset;

			pitch = std::clamp(pitch, -89.0f, 89.0f);

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			camera->front = glm::normalize(front);
		}

		void Update(float deltaTime) {
			float velocity = moveSpeed * deltaTime;

			if (Input::KeyW() == true)
				transform->position += camera->front * velocity;

			if (Input::KeyS() == true)
				transform->position -= camera->front * velocity;

			glm::vec3 right = glm::normalize(glm::cross(camera->front, camera->up));

			if (Input::KeyA() == true)
				transform->position -= right * velocity;

			if (Input::KeyD() == true)
				transform->position += right * velocity;
		}

		void SetMoveSpeed(float speed) { moveSpeed = speed; }
		void SetSensitivity(float sens) { lookSensitivity = sens; }
	};

}

#endif //CAMERACONTROLLERSYSTEM_HPP
