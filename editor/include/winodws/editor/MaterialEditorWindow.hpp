//
// Created by arunc on 01/07/2025.
//

#ifndef MATERIALEDITORWINDOW_HPP
#define MATERIALEDITORWINDOW_HPP
#include <imgui.h>
#include <memory>
#include "gui/material/MaterialManager.hpp"
#include "gui/material/Material.hpp"
#include "service/EditorService.hpp"
#include "service/AppContext.hpp"
#include "io/MaterialIO.hpp"
#include "service/AssetsService.hpp"
#include <cstdio>

class MaterialEditorWindow {
public:
	MaterialEditorWindow()
	{

	}

	void Render() {

		if (Service::EditorService::Instance().getMaterialSelected() && path != Service::EditorService::Instance().getMaterialSelected()->path) {
			path = Service::EditorService::Instance().getMaterialSelected()->path;
			material = Material::MaterialManager::Get(Service::EditorService::Instance().getMaterialSelected()->id);
		}
		if (!material)
			return;

		ImGui::Begin(("Material Editor: " + material->name).c_str());

		ImGui::Text("Path: %s", path.c_str());
		ImGui::Separator();

		static const char* shaderTypes[] = { "default", "phong", "pbr" };
		int currentShader = 0;

		if (std::dynamic_pointer_cast<Material::MaterialPhong>(material)) currentShader = 1;
		else if (std::dynamic_pointer_cast<Material::MaterialPBR>(material)) currentShader = 2;

		if (ImGui::Combo("Shader Type", &currentShader, shaderTypes, IM_ARRAYSIZE(shaderTypes))) {
			// Convertește tipul
			std::shared_ptr<Material::MaterialBase> newMaterial;

			if (currentShader == 0) {
				auto def = std::make_shared<Material::MaterialDefault>();
				def->name = material->name;
				newMaterial = def;
			}
			else if (currentShader == 1) {
				auto phong = std::make_shared<Material::MaterialPhong>();
				phong->name = material->name;
				newMaterial = phong;
			}
			else if (currentShader == 2) {
				auto pbr = std::make_shared<Material::MaterialPBR>();
				pbr->name = material->name;
				newMaterial = pbr;
			}

			if (newMaterial) {
				newMaterial->id = material->id; // Păstrează ID-ul
				material = newMaterial;
				Material::MaterialManager::Register(Service::EditorService::Instance().getMaterialSelected()->id, material);
			}

		}

		ImGui::Separator();


		if (auto phong = std::dynamic_pointer_cast<Material::MaterialPhong>(material)) {
			RenderPhong(phong);
			material = phong;
		}
		else if (auto pbr = std::dynamic_pointer_cast<Material::MaterialPBR>(material)) {
			RenderPBR(pbr);
			material = pbr;
		}
		else if (auto def = std::dynamic_pointer_cast<Material::MaterialDefault>(material)) {
			RenderDefault(def);
			material = def;
		}

		if (ImGui::Button("Save")) {
			MaterialIO().Save(AppContext::Instance().GetCurrentProject().path + "/" + Service::EditorService::Instance().getMaterialSelected()->path, material);
		}

		ImGui::End();
	}

private:
	std::string path;
	std::shared_ptr<Material::MaterialBase> material;


	void GetTexture(const char* label, std::string& path) {
		std::vector<Model::AssetItem> textures;
		std::string defaultTexture = "";
		for (const auto& asset : Service::AssetsService::Instance().getAssets()) {
			if (asset.type == Model::AssetType::Texture) {
				textures.push_back(asset);
				if (asset.id == path) {
					defaultTexture = asset.name;
				}
			}
		}



		static int selectedIndex = -1;
		if (ImGui::BeginCombo(("Texture##" + std::string(label)).c_str(), defaultTexture.c_str())) {
			for (int i = 0; i < textures.size(); ++i) {
				bool isSelected = (path == textures[i].path);
				if (ImGui::Selectable(textures[i].name.c_str(), isSelected)) {
					path = textures[i].id;
					selectedIndex = i;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}


	void RenderColorOrTexture(const char* label, ColorOrTexture& cot) {
		ImGui::Text("%s", label);
		std::string pp = "Is Texture##" + std::string(label);
		ImGui::Checkbox(pp.c_str(), &cot.isTexture);
		if (cot.isTexture) {
			ImGui::Text("%s", label);
			GetTexture(label, cot.texturePath);
		}
		else {
			float color[3] = { 1.0f, 1.0f, 1.0f };

			if (cot.color.size() == 7 && cot.color[0] == '#') {
				int r, g, b;
				sscanf_s(cot.color.c_str(), "#%02x%02x%02x", &r, &g, &b);
				color[0] = r / 255.0f;
				color[1] = g / 255.0f;
				color[2] = b / 255.0f;
			}
			std::string idColor = "Color##" + std::string(label);
			if (ImGui::ColorEdit3(idColor.c_str(), color)) {
				char buffer[8];
				snprintf(buffer, sizeof(buffer), "#%02x%02x%02x",
					(int)(color[0] * 255),
					(int)(color[1] * 255),
					(int)(color[2] * 255));
				cot.color = buffer;
			}
		}
	}

	void RenderPhong(const std::shared_ptr<Material::MaterialPhong>& mat) {
		ImGui::SliderFloat("Shininess", &mat->shininess, 0.0f, 256.0f);
		RenderColorOrTexture("Ambient", mat->ambient);
		RenderColorOrTexture("Diffuse", mat->diffuse);
		RenderColorOrTexture("Specular", mat->specular);
	}

	void RenderPBR(const std::shared_ptr<Material::MaterialPBR>& mat)
	{
		RenderColorOrTexture("Albedo", mat->albedo);

		ImGui::Text("%s", "Normal");
		GetTexture("Normal", mat->normal);

		ImGui::Text("%s", "Metallic");
		std::string pp = "Is Texture##" + std::string("Metallic");
		ImGui::Checkbox(pp.c_str(), &mat->metallic.isTexture);
		if (mat->metallic.isTexture) {
			GetTexture("Metallic", mat->metallic.texturePath);
		}
		else {
			ImGui::SliderFloat("Metallic", &mat->metallic.value, 0.0f, 1.0f, "%.02f");
		}


		ImGui::Text("%s", "Roughness");
		pp = "Is Texture##" + std::string("Roughness");
		ImGui::Checkbox(pp.c_str(), &mat->roughness.isTexture);
		if (mat->roughness.isTexture) {
			GetTexture("Roughness", mat->roughness.texturePath);
		}
		else {
			ImGui::SliderFloat("Roughness", &mat->roughness.value, 0.0f, 1.0f, "%.02f");
		}


		ImGui::Text("%s", "AO");
		pp = "Is Texture##" + std::string("AO");
		ImGui::Checkbox(pp.c_str(), &mat->ao.isTexture);
		if (mat->ao.isTexture) {
			GetTexture("AO", mat->ao.texturePath);
		}
		else {
			ImGui::SliderFloat("AO", &mat->ao.value, 0.0f, 1.0f, "%.02f");
		}

	}

	void RenderDefault(const std::shared_ptr<Material::MaterialDefault>& mat) {
		ImGui::SliderFloat("Shininess", &mat->shininess, 0.0f, 256.0f);
		ImGui::ColorEdit3("Diffuse Color", mat->diffuseColor);
		ImGui::ColorEdit3("Specular Color", mat->specularColor);
	}
};

#endif //MATERIALEDITORWINDOW_HPP
