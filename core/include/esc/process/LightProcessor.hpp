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
#include "gui/shader/ShaderBinder.hpp"
#include "gui/shader/ShaderManager.hpp"
#include "gui/shader/ShaderProgram.hpp"
#include "gui/texture/Texture.hpp"

namespace Process {
    class LightProcessor {
    public:
        GLuint pointIcon;
        GLuint directIcon;
        GLuint spotIcon;

        GLuint vao = 0, vbo = 0, ebo = 0;

        LightProcessor() {
            pointIcon = Texture::LoadTexture("assets/bulb.png", false);
            directIcon = Texture::LoadTexture("assets/sun.png", false);
            spotIcon = Texture::LoadTexture("assets/spotlight.png", false);

            InitQuad();
        }

        ~LightProcessor() {
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
            glDeleteVertexArrays(1, &vao);
        }

        std::vector<std::shared_ptr<Entity::Entity>> GetLights(const World& scene) {
            std::vector<std::shared_ptr<Entity::Entity>> lights;
            for (const auto& entity : scene.GetEntities()) {
                if (entity->HasComponent<Component::LightComponent>())
                    lights.push_back(entity);
            }
            return lights;
        }

        void RenderLights(const std::vector<std::shared_ptr<Entity::Entity>>& lights, const glm::mat4& view,
                          const glm::mat4& projection, const glm::vec3& camPos, const glm::vec3& camUp,
                          const glm::vec3& camFront) {
            ShaderProgram shader(ShaderManager::Get(ShaderType::Gizmo));
            shader.Use();
            ShaderBinder binder(shader.GetProgramId());

            binder.setMat4("uView", view);
            binder.setMat4("uProjection", projection);

            for (const auto& entity : lights) {
                auto light = entity->GetComponent<Component::LightComponent>();
                auto transform = entity->GetComponent<Component::TransformComponent>();
                if (!light || !transform) continue;

                glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->position);
                model *= glm::mat4(glm::mat3(glm::inverse(view)));

                binder.setMat4("uModel", model);

                glActiveTexture(GL_TEXTURE0);
                switch (light->Type) {
                    case Light::LightType::Directional:
                        glBindTexture(GL_TEXTURE_2D, directIcon);
                        break;
                    case Light::LightType::Spot:
                        glBindTexture(GL_TEXTURE_2D, spotIcon);
                        break;
                    case Light::LightType::Point:
                    default:
                        glBindTexture(GL_TEXTURE_2D, pointIcon);
                        break;
                }

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
            }
        }

    private:
        void InitQuad() {
            float size = 0.5f;
            float vertices[] = {
                -size, -size, 0.f, 0.f, 0.f,
                 size, -size, 0.f, 1.f, 0.f,
                 size,  size, 0.f, 1.f, 1.f,
                -size,  size, 0.f, 0.f, 1.f,
            };
            uint32_t indices[] = {0, 1, 2, 2, 3, 0};

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }
    };
}
#endif //LIGHTPROCESSOR_HPP
