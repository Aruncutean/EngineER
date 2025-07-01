//
// Created by arunc on 01/07/2025.
//

#ifndef ENTITYSERVICE_HPP
#define ENTITYSERVICE_HPP

#include "io/MeshDataSerializer.hpp"
#include <fstream>
#include "esc/entity/Entity.hpp"
#include "esc/component/MeshComponent.hpp"
#include "esc/component/TransformComponent.hpp"
#include "esc/component/MaterialComponent.hpp"
#include "esc/component/PhysicsComponent.hpp"

namespace Service {

    class EntityService
    {
    public:
        Entity::Entity* create(std::string meshPath) {
            Entity::Entity* entity = new Entity::Entity();
            entity->AddComponent<Component::MaterialComponent>();
            entity->AddComponent<Component::TransformComponent>();
            entity->AddComponent<Component::MeshComponent>();
            entity->AddComponent<Component::PhysicsComponent>();

            auto shaderComponent = entity->GetComponent<Component::MaterialComponent>();
            shaderComponent->MaterialID = "default";

            auto transformComponent = entity->GetComponent<Component::TransformComponent>();
            transformComponent->position = glm::vec3(0);
            transformComponent->scale = glm::vec3(1, 1, 1);

            auto meshComponent = entity->GetComponent<Component::MeshComponent>();
            meshComponent->meshPath = meshPath;

            return entity;
        }


    private:

    };
}


#endif //ENTITYSERVICE_HPP
