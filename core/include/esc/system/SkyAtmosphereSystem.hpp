//
// Created by arunc on 01/07/2025.
//

#ifndef SKYATMOSPHERESYSTEM_HPP
#define SKYATMOSPHERESYSTEM_HPP

#include <glad/glad.h>
#include <glm.hpp>
#include <string>
#include "gui/shader/ShaderProgram.hpp"
#include "gui/FullscreenQuad.hpp"
#include "gui/shader/ShaderBinder.hpp"
#include "gui/shader/ShaderTypes.hpp"
#include "gui/shader/ShaderManager.hpp"
namespace System {


    class SkyAtmosphereSystem {
    public:
        SkyAtmosphereSystem() : fullscreenQuad(0) {}

        bool Load() {
            fullscreenQuad = FullscreenQuad::Create();
            return true;
        }

        void Update(const glm::vec3& cameraPos) {

        }

        void Render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos, const glm::vec3& sunDir) {
            glDepthFunc(GL_LEQUAL);

            ShaderProgram shaderProgram(ShaderManager::Get(ShaderType::SkyAtmosphere));
            shaderProgram.Use();
            ShaderBinder binder(shaderProgram.GetProgramId());


            glm::mat4 invVP = glm::inverse(projection * view);

            binder.setMat4("inverseViewProjection", invVP);
            binder.setVec3("cameraPos", cameraPos);
            binder.setVec3("sunDirection", sunDir);


            fullscreenQuad->Render();

            glDepthFunc(GL_LESS);
        }

    private:

        std::shared_ptr<FullscreenQuad> fullscreenQuad;
    };


}
#endif //SKYATMOSPHERESYSTEM_HPP
