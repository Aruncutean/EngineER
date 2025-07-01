//
// Created by arunc on 01/07/2025.
//

#ifndef RENDERABLEPROCESSOR_HPP
#define RENDERABLEPROCESSOR_HPP

#include <vector>
#include <memory>
#include "model/World.hpp"
#include "esc/entity/Entity.hpp"
#include "esc/component/TransformComponent.hpp"
#include "esc/component/CameraComponent.hpp"
#include "esc/component/MeshComponent.hpp"
#include "esc/component/MaterialComponent.hpp"
#include "esc/component/LightComponent.hpp"
#include "gui/shader/ShaderBinder.hpp"
#include "gui/material/MaterialManager.hpp"
#include "gui/shader/ShaderManager.hpp"
#include "gui/mesh/GLMesh.hpp"
#include "gui/ColorUtils.hpp"
#include "gui/texture/TextureManager.hpp"
#include "gui/shader/ShaderProgram.hpp"
#include "gui/light/Attenuation.hpp"
#include "gui/light/Light.hpp"
#include "gui/material/Material.hpp"

class RenderableProcessor {
public:
	std::vector<std::shared_ptr<Entity::Entity>> GetRenderables(const World& scene) {
		std::vector<std::shared_ptr<Entity::Entity>> result;
		for (auto& e : scene.GetEntities()) {
			if (e->HasComponent<Component::MeshComponent>() &&
				e->HasComponent<Component::MaterialComponent>() &&
				!e->HasComponent<Component::LightComponent>()) {
				result.push_back(e);
			}
		}
		return result;
	}

	void RenderEntity(std::shared_ptr<Entity::Entity> entity,
		const std::vector<std::shared_ptr<Entity::Entity>>& lights,
		const glm::mat4& view, const glm::mat4& projection,
		const Component::TransformComponent& cameraTransform) {
		auto transform = entity->GetComponent<Component::TransformComponent>();
		auto meshComp = entity->GetComponent<Component::MeshComponent>();
		auto matComp = entity->GetComponent<Component::MaterialComponent>();

		if (!transform || !meshComp || !matComp) return;

		auto material = Material::MaterialManager::Get(matComp->MaterialID);
		if (!material) return;

		ShaderProgram shaderProgram(ShaderManager::Get(material->getShader()));
		shaderProgram.Use();
		ShaderBinder binder(shaderProgram.GetProgramId());

		glm::mat4 model = transform->GetTransform();

		binder.setMat4("model", model);
		binder.setMat4("view", view);
		binder.setMat4("projection", projection);

		if (auto phong = std::dynamic_pointer_cast<Material::MaterialPhong>(material)) {
			binder.setVec3("viewPos", cameraTransform.position);
			binder.setInt("material.ambientMap", 0);
			binder.setInt("material.diffuseMap", 1);
			binder.setInt("material.specularMap", 2);

			// Set ambient
			if (!phong->ambient.isTexture) {
				binder.setVec3("material.ambientColor", ColorUtils::HexToVec3(phong->ambient.color));
				binder.setInt("material.useAmbientMap", 0);
			}
			else {
				binder.setInt("material.useAmbientMap", 1);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(phong->ambient.texturePath, true));

			}

			// Diffuse
			if (!phong->diffuse.isTexture) {
				binder.setVec3("material.diffuseColor", ColorUtils::HexToVec3(phong->diffuse.color));
				binder.setInt("material.useDiffuseMap", 0);
			}
			else {
				binder.setInt("material.useDiffuseMap", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(phong->diffuse.texturePath, false));
			}

			// Specular
			if (!phong->specular.isTexture) {
				binder.setVec3("material.specularColor", ColorUtils::HexToVec3(phong->specular.color));
				binder.setInt("material.useSpecularMap", 0);
			}
			else {
				binder.setInt("material.useSpecularMap", 1);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(phong->specular.texturePath, false));
			}

			binder.setFloat("material.shininess", phong->shininess);

			int i = 0;
			for (const auto& light : lights) {
				auto lightComp = light->GetComponent<Component::LightComponent>();
				auto lightTransform = light->GetComponent<Component::TransformComponent>();

				if (!lightComp || !lightTransform) continue;

				if (auto dir = std::dynamic_pointer_cast<Light::LightDirectional>(lightComp->LightBase)) {
					glm::vec3 dirVec = glm::normalize(transform->position - lightTransform->position);
					binder.setVec3("dirLight.direction", dirVec);
					binder.setVec3("dirLight.color", dir->color);
					binder.setFloat("dirLight.intensity", dir->intensity);
				}
				else if (auto pt = std::dynamic_pointer_cast<Light::LightPoint>(lightComp->LightBase)) {
					float constant, linear, quadratic;
					Attenuation::FromRange(pt->range, constant, linear, quadratic);

					std::string prefix = "pointLights[" + std::to_string(i) + "]";
					binder.setVec3(prefix + ".position", lightTransform->position);
					binder.setVec3(prefix + ".color", pt->color);
					binder.setFloat(prefix + ".intensity", pt->intensity);
					binder.setFloat(prefix + ".constant", constant);
					binder.setFloat(prefix + ".linear", linear);
					binder.setFloat(prefix + ".quadratic", quadratic);
					i++;
				}
			}

			binder.setInt("numPointLights", i);
		}
		else if (auto pbr = std::dynamic_pointer_cast<Material::MaterialPBR>(material)) {
			binder.setVec3("camPos", cameraTransform.position);
			binder.setInt("material.albedoMap", 0);
			binder.setInt("material.normalMap", 1);
			binder.setInt("material.metallicMap", 2);
			binder.setInt("material.roughnessMap", 3);
			binder.setInt("material.aoMap", 4);

			// Albedo
			binder.setInt("material.useAlbedoMap", pbr->albedo.isTexture ? 1 : 0);
			if (!pbr->albedo.isTexture) {
				binder.setVec3("material.albedoColor", ColorUtils::HexToVec3(pbr->albedo.color));
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(pbr->albedo.texturePath, true));
			}

			// Normal
			binder.setInt("material.useNormalMap", 0);
			if (pbr->normal != "") {
				binder.setInt("material.useNormalMap", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(pbr->normal, false));
			}


			// metallic
			binder.setInt("material.useMetallicMap", pbr->metallic.isTexture ? 1 : 0);
			if (!pbr->metallic.isTexture) {
				binder.setFloat("material.metallic", pbr->metallic.value);
			}
			else {
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(pbr->metallic.texturePath, false));
			}

			// roughness
			binder.setInt("material.useRoughnessMap", pbr->roughness.isTexture ? 1 : 0);
			if (!pbr->roughness.isTexture) {
				binder.setFloat("material.roughness", pbr->roughness.value);
			}
			else {
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(pbr->roughness.texturePath, false));
			}

			// AO
			binder.setInt("material.useAOMap", pbr->ao.isTexture ? 1 : 0);
			if (!pbr->ao.isTexture) {
				binder.setFloat("material.aoValue", pbr->ao.value);
			}
			else {
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, Material::TextureManager::Get(pbr->ao.texturePath, false));
			}

			int i = 0;
			for (const auto& light : lights) {
				auto lightComp = light->GetComponent<Component::LightComponent>();
				auto lightTransform = light->GetComponent<Component::TransformComponent>();

				if (!lightComp || !lightTransform) continue;

				if (auto dir = std::dynamic_pointer_cast<Light::LightDirectional>(lightComp->LightBase)) {
					glm::vec3 dirVec = glm::normalize(transform->position - lightTransform->position);
					binder.setVec3("dirLight.direction", dirVec);
					binder.setVec3("dirLight.color", dir->color);
					binder.setFloat("dirLight.intensity", dir->intensity);
				}
				else if (auto pt = std::dynamic_pointer_cast<Light::LightPoint>(lightComp->LightBase)) {
					float constant, linear, quadratic;
					Attenuation::FromRange(pt->range, constant, linear, quadratic);

					std::string prefix = "pointLights[" + std::to_string(i) + "]";
					binder.setVec3(prefix + ".position", lightTransform->position);
					binder.setVec3(prefix + ".color", pt->color);
					binder.setFloat(prefix + ".intensity", pt->intensity);


					i++;
				}
			}
			binder.setInt("numPointLights", i);
		}

		if (!meshComp->glMesh)
			meshComp->LoadMesh();

		meshComp->glMesh->Render();
	}
};

#endif //RENDERABLEPROCESSOR_HPP
