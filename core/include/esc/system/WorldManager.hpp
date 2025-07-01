//
// Created by arunc on 01/07/2025.
//

#ifndef WORLDMANAGER_HPP
#define WORLDMANAGER_HPP
#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm.hpp>
#include "gui/framebuffer/FrameBuffer.hpp"
#include "esc/entity/Entity.hpp"
#include "model/World.hpp"
#include "model/SceneInfo.hpp"
#include "RenderSystem.hpp"
#include "CameraControllerSystem.hpp"
#include "gui/shader/ShaderManager.hpp"
#include "io/SceneSerializer.hpp"
#include "esc/component/TransformComponent.hpp"
#include "esc/component/CameraComponent.hpp"
#include "esc/component/CameraControllerComponent.hpp"
#include "service/WindowService.hpp"
#include "PhysicsSystem.hpp"
#include "TerrainSystem.hpp"
#include "SkyAtmosphereSystem.hpp"

class WorldManager {
public:
	WorldManager(const std::string& path)
		: projectPath(path)
	{
		Init();
	}

	void Init() {
		ShaderManager::LoadShaders();

		cameraEntity = std::make_shared<Entity::Entity>();
		cameraEntity->AddComponent<Component::TransformComponent>();
		cameraEntity->AddComponent<Component::CameraComponent>();
		cameraEntity->AddComponent<Component::CameraControllerComponent>();

		cameraEntity->GetComponent<Component::TransformComponent>()->position = glm::vec3(0, 5, 10);
		cameraEntity->GetComponent<Component::CameraComponent>()->up = glm::vec3(0, 1, 0);

		renderSystem = std::make_unique<System::RenderSystem>();
		renderSystem->cameraEntity = cameraEntity;

		Service::EditorService::Instance().setEditorCamera(cameraEntity.get());

		cameraController = std::make_shared<System::CameraControllerSystem>(cameraEntity);
		cameraController->InitCallbacks();
		physicsSysyem = std::make_shared<System::PhysicsSystem>();
		physicsSysyem->InitCallbacks();

		frameBuffer = std::make_unique<FrameBuffer>(
			500,
			1200
		);

		terrainSystem = std::make_shared<System::TerrainSystem>();


		skyAtmosphereSystem = std::make_shared<System::SkyAtmosphereSystem>();
		skyAtmosphereSystem->Load();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		AppContext::Instance().addEntity = [this](std::shared_ptr<Entity::Entity> entity) {
			physicsSysyem->AddPhysicsBody(entity);
			};
	}

	void Resize(int width, int height) {
		if (frameBuffer)
			frameBuffer->Resize(width, height);
	}

	void LoadWorld(const Model::SceneInfo& info) {
		std::string path = projectPath + "/scenes/" + info.path;
		currentWorld = std::make_shared<World>(SceneSerializer::LoadScene(path));
		//terrainSystem->Load(projectPath + '/' + currentWorld.get()->GeTerrainPath(), 1.0f, 20.0f, 16);
		AppContext::Instance().SetCurrentWorld(*currentWorld);
		InitPhysics();
	}

	void LoadWorld(const std::string& path) {
		currentWorld = std::make_shared<World>(SceneSerializer::LoadScene(path));
		//terrainSystem->Load(projectPath + '/' + currentWorld.get()->GeTerrainPath(), 1.0f, 20.0f, 16);
		AppContext::Instance().SetCurrentWorld(*currentWorld);
		InitPhysics();

	}

	void LoadWorld(std::shared_ptr<World> world) {
		currentWorld = world;
		//terrainSystem->Load(projectPath + '/' + currentWorld.get()->GeTerrainPath(), 1.0f, 20.0f, 16);
		AppContext::Instance().SetCurrentWorld(*currentWorld);
		InitPhysics();
	}

	void Update(float deltaTime) {
		if (cameraController && currentWorld) {

			glm::vec3 camPos = cameraEntity->GetComponent<Component::TransformComponent>()->position;
			//	terrainSystem->Update(camPos);
			//	terrainSystem->ProcessPendingLODs(1550);
			cameraController->Update(deltaTime);

		}
	}

	void InitPhysics() {
		if (physicsSysyem) {
			for (const auto& entity : currentWorld.get()->GetEntities()) {
				physicsSysyem->AddPhysicsBody(entity);
			}
		}
	}

	void Render(float deltaTime) {
		auto renderScene = [&]() {
			glClearColor(0.247f, 0.247f, 0.247f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDisable(GL_DEPTH_TEST);


			if (skyAtmosphereSystem) {
				auto transform = cameraEntity->GetComponent<Component::TransformComponent>();
				auto cameraComp = cameraEntity->GetComponent<Component::CameraComponent>();

				skyAtmosphereSystem->Render(
					Service::EditorService::Instance().getView(),
					Service::EditorService::Instance().getProjection(),
					transform->position,
					glm::vec3(0, 1, -1)
				);
			}

			glEnable(GL_DEPTH_TEST);

			if (currentWorld) {



				auto currentWorldPath = AppContext::Instance().GetCurrentWorld();

				if (physicsSysyem) {
					physicsSysyem->Step(&currentWorldPath, deltaTime);
				}

				if (renderSystem) {
					renderSystem->Render(&currentWorldPath, WindowService::getInstance()->getWidth(),
						WindowService::getInstance()->getHeight());
				}

				/*if (terrainSystem)
				{
					glm::vec3 camPos = cameraEntity->GetComponent<Component::TransformComponent>()->position;
					terrainSystem->Render(Service::EditorService::Instance().getView(), Service::EditorService::Instance().getProjection(), camPos);
				}*/

			}
			};

		if (renderInFrameBuffer && frameBuffer) {
			frameBuffer->Render(renderScene);
			frameBuffer->BlitToTexture();
		}
		else {
			renderScene();
		}
	}

	void Dispose() {
		frameBuffer.reset();
		renderSystem.reset();
		cameraController.reset();
	}

	bool renderInFrameBuffer = false;
	bool isEditMode = false;

	std::shared_ptr<World> currentWorld;
	std::shared_ptr<Entity::Entity> cameraEntity;
	std::unique_ptr<System::RenderSystem> renderSystem;
	std::shared_ptr<System::PhysicsSystem> physicsSysyem;
	std::shared_ptr<System::CameraControllerSystem> cameraController;
	std::shared_ptr<System::TerrainSystem> terrainSystem;
	std::shared_ptr<System::SkyAtmosphereSystem> skyAtmosphereSystem;
	std::unique_ptr<FrameBuffer> frameBuffer;
	std::string projectPath;
};

#endif //WORLDMANAGER_HPP
