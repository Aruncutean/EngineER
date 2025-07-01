//
// Created by arunc on 01/07/2025.
//

#ifndef VIEWSCENEWINDOW_HPP
#define VIEWSCENEWINDOW_HPP
#include <imgui.h>
#include <vector>
#include <functional>
#include <memory>

#include "Window.hpp"
#include "MainWindow.hpp"
#include "esc/system/WorldManager.hpp"
#include "service/WindowService.hpp"
#include "service/AppContext.hpp"
#include <ImGuizmo.h>
#include "service/EditorService.hpp"

class ViewSceneWindow : public Window, public std::enable_shared_from_this<ViewSceneWindow>
{
public:
	ViewSceneWindow(MainWindow* host) : host(host) {
		host->IsBockingSpace = true;

		std::optional<Model::ProjectData> projectData = AppContext::Instance().GetCurrentProject();


		_worldSystem = new WorldManager(projectData->path);
		_worldSystem->isEditMode = true;
		_worldSystem->renderInFrameBuffer = true;
		_worldSystem->Init();
		_worldSystem->LoadWorld(AppContext::Instance().GetCurrentScene().path);

	}

	~ViewSceneWindow() {

	}

	void Render() override {

		_worldSystem->Update(WindowService::getInstance()->getDeltaTime());


		bool isOpen = true;
		UI::Window("#Scene Scene", isOpen, [&]()
			{
				ImVec2 size = ImGui::GetContentRegionAvail();
				if (isGuizmoClicked == false) {
					Input::Instance().isActive = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
				}

				ImVec2 contentMin = ImGui::GetCursorScreenPos();
				ImVec2 mousePos = ImGui::GetMousePos();
				ImVec2 relativeMousePos = ImVec2(mousePos.x - contentMin.x, mousePos.y - contentMin.y);

				Input::MouseMoveCall(glm::vec2(relativeMousePos.x, relativeMousePos.y));


				if (size.y!=0 &&  size.x!=0 && ( (int)size.x != framebufferWidth || (int)size.y != framebufferHeight))
				{

					WindowService::getInstance()->setHeight((int)size.y);
					WindowService::getInstance()->setWidth((int)size.x);
					framebufferHeight = (int)size.y;
					framebufferWidth = (int)size.x;
					_worldSystem->Resize((int)size.x, (int)size.y);
				}
				_worldSystem->Render(WindowService::getInstance()->getDeltaTime());


				_worldSystem->frameBuffer->BlitToTexture();


				if (_worldSystem->frameBuffer != nullptr)
				{
					ImGui::Image(_worldSystem->frameBuffer->_texture, size, ImVec2(0, 1),
						ImVec2(1, 0));
				}

				ImGuizmo::SetDrawlist();

				ImVec2 windowPos = ImGui::GetWindowPos();
				ImVec2 regionMin = ImGui::GetWindowContentRegionMin();
				ImVec2 regionMax = ImGui::GetWindowContentRegionMax();

				ImVec2 min = ImVec2(windowPos.x + regionMin.x, windowPos.y + regionMin.y);
				ImVec2 size1 = ImVec2(regionMax.x - regionMin.x, regionMax.y - regionMin.y);

				ImGuizmo::SetRect(min.x, min.y, size1.x, size1.y);

				glm::mat4 identity = glm::mat4(1.0f);

				if (ImGui::BeginPopupContextWindow("scene_context", ImGuiPopupFlags_MouseButtonRight)) {
					if (ImGui::MenuItem("Add Light")) {

						auto lightEntity = new Entity::Entity();
						lightEntity->Name = "Light";

						lightEntity->AddComponent<Component::TransformComponent>();
						lightEntity->AddComponent<Component::LightComponent>();

						auto lightComp = lightEntity->GetComponent<Component::LightComponent>();
						lightComp->Type = Light::LightType::Point;
						lightComp->LightBase = std::make_shared<Light::LightPoint>();

						auto transform = lightEntity->GetComponent<Component::TransformComponent>();
						transform->position = glm::vec3(0.0f, 5.0f, 0.0f);

						AppContext::Instance().GetCurrentWorld().AddEntity(std::shared_ptr<Entity::Entity>(lightEntity));
					}
					if (ImGui::MenuItem("Add Direct Light")) {

						auto lightEntity = new Entity::Entity();
						lightEntity->Name = "Light Direct";

						lightEntity->AddComponent<Component::TransformComponent>();
						lightEntity->AddComponent<Component::LightComponent>();

						auto lightComp = lightEntity->GetComponent<Component::LightComponent>();
						lightComp->Type = Light::LightType::Point;
						lightComp->LightBase = std::make_shared<Light::LightDirectional>();

						auto transform = lightEntity->GetComponent<Component::TransformComponent>();
						transform->position = glm::vec3(0.0f, 5.0f, 0.0f);

						AppContext::Instance().GetCurrentWorld().AddEntity(std::shared_ptr<Entity::Entity>(lightEntity));
					}
					ImGui::EndPopup();
				}
			   ImGui::Text("Gizmo USING: %s", ImGuizmo::IsUsing() ? "YES" : "NO");
				if (auto selectedEntity = Service::EditorService::Instance().GetSelectedEntity()) {
					if (selectedEntity->HasComponent<Component::TransformComponent>()) {
						auto transform = selectedEntity->GetComponent<Component::TransformComponent>();
						auto physicsComponent = selectedEntity->GetComponent<Component::PhysicsComponent>();
						glm::mat4 model = transform->GetTransform();

						if (Input::KeyT()) currentOp = ImGuizmo::TRANSLATE;
						if (Input::KeyR()) currentOp = ImGuizmo::ROTATE;
						if (Input::KeyS()) currentOp = ImGuizmo::SCALE;
						glm::mat4 view = Service::EditorService::Instance().getView();

						ImGuizmo::Manipulate(
							glm::value_ptr(view),
							glm::value_ptr(Service::EditorService::Instance().getProjection()),
							currentOp,
							ImGuizmo::WORLD,
							glm::value_ptr(model)
						);

						if (ImGuizmo::IsUsing()) {
							Input::Instance().isActive = false;
							isGuizmoClicked = true;
							glm::vec3 translation, scale;
							glm::vec3 eulerDegrees;
							ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(model),
								glm::value_ptr(translation),
								glm::value_ptr(eulerDegrees),
								glm::value_ptr(scale));


							transform->position = translation;
							transform->rotation = glm::quat(glm::radians(eulerDegrees));
							transform->scale = scale;

							if (physicsComponent) {
								btTransform newTransform;
								newTransform.setOrigin(btVector3(translation.x, translation.y, translation.z));

								glm::quat q = transform->rotation;
								btQuaternion btRot(q.x, q.y, q.z, q.w);
								newTransform.setRotation(btRot);

								physicsComponent->rigidBody->setWorldTransform(newTransform);
								if (physicsComponent->motionState)
									physicsComponent->motionState->setWorldTransform(newTransform);

								if (btCollisionShape* shape = physicsComponent->rigidBody->getCollisionShape()) {
									shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
								}
							}
						}
						else {
							isGuizmoClicked = false;
						}
					}
				}
			});

	}




private:
	MainWindow* host = nullptr;
	WorldManager* _worldSystem = nullptr;
	int framebufferWidth;
	int framebufferHeight;
	bool isGuizmoClicked = false;
	ImGuizmo::OPERATION currentOp = ImGuizmo::TRANSLATE;
};

#endif //VIEWSCENEWINDOW_HPP
