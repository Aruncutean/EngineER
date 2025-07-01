//
// Created by arunc on 01/07/2025.
//

#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP
#pragma once

#include "CameraSystem.hpp"
#include "esc/process/LightProcessor.hpp"
#include "esc/process/RenderableProcessor.hpp"
#include "esc/entity/Entity.hpp"
#include "model/World.hpp"
#include "glad/glad.h"
#include <memory>
#include "service/EntityService.hpp"

namespace System {

    class RenderSystem {
    public:
        std::shared_ptr<Entity::Entity> cameraEntity;

        RenderSystem() :
            cameraSystem(std::make_unique<CameraSystem>()),
            lightProcessor(std::make_unique<Process::LightProcessor>()),
            renderableProcessor(std::make_unique<RenderableProcessor>()) {
        }

        void Render(World* scene, int width, int height) {
            if (!cameraEntity)
                return;

            auto camera = cameraEntity->GetComponent<Component::CameraComponent>();
            auto transform = cameraEntity->GetComponent<Component::TransformComponent>();


            if (!camera || !transform)
                return;



            glm::mat4 view = cameraSystem->GetViewMatrix(*transform, *camera);
            glm::mat4 projection = cameraSystem->GetProjectionMatrix(*camera, static_cast<float>(width) / height);

            Service::EditorService::Instance().setProjection(projection);
            Service::EditorService::Instance().setView(view);

            auto lights = lightProcessor->GetLights(*scene);
            auto renderables = renderableProcessor->GetRenderables(*scene);


            for (const auto& entity : renderables) {
                renderableProcessor->RenderEntity(entity, lights, view, projection, *transform);
            }



        }

    private:
        std::unique_ptr<CameraSystem> cameraSystem;
        std::unique_ptr<Process::LightProcessor> lightProcessor;
        std::unique_ptr<RenderableProcessor> renderableProcessor;
    };

}

#endif //RENDERSYSTEM_HPP
