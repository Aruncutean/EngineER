//
// Created by arunc on 01/07/2025.
//

#ifndef LIGHTPROCESSOR_HPP
#define LIGHTPROCESSOR_HPP


#include <vector>
#include <memory>
#include "model/World.hpp"
#include "esc/entity/Entity.hpp"
#include "esc/component/LightComponent.hpp"

namespace Process {

    class LightProcessor {
    public:
        std::vector<std::shared_ptr<Entity::Entity>> GetLights(const World& scene) {
            std::vector<std::shared_ptr<Entity::Entity>> lights;

            for (const auto& entity : scene.GetEntities()) {
                if (entity->HasComponent<Component::LightComponent>()) {
                    lights.push_back(entity);
                }
            }

            return lights;
        }
    };

}

#endif //LIGHTPROCESSOR_HPP
