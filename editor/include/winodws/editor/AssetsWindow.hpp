//
// Created by arunc on 01/07/2025.
//

#ifndef ASSETSWINDOW_HPP
#define ASSETSWINDOW_HPP
#include <imgui.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "Window.hpp"
#include "MainWindow.hpp"
#include "service/AssetsService.hpp"
#include <glad/glad.h>
#include "entity/LoadEntity.hpp"
#include <iostream>
#include <filesystem>
#include "esc/component/MeshComponent.hpp"
#include "esc/component/TransformComponent.hpp"
#include "esc/entity/Entity.hpp"
#include "esc/component/MaterialComponent.hpp"
#include "service/EditorService.hpp"
#include "MaterialEditorWindow.hpp"
#include "gui/texture/TextureManager.hpp"
#include "service/EntityService.hpp"

class AssetsWindow : public Window, public std::enable_shared_from_this<AssetsWindow>
{
public:
	AssetsWindow(MainWindow* host) : host(host) {
		host->IsBockingSpace = true;
		addIcon();
	}

	~AssetsWindow() {

	}

	void Render() override {
		bool isOpen = true;
		UI::Window("Assets", isOpen, [&]()
			{
				RenderBreadcrumb(currentFolder, [&](const std::string& newPath)
					{
						currentFolder = newPath;
					}
				);


				std::vector<Model::AssetItem> allAssets = Service::AssetsService::Instance().getAssets();
				std::vector<Model::AssetItem> assetItems;

				for (const auto& a : allAssets) {
					if (a.baseDirectory == currentFolder)
						assetItems.push_back(a);
				}

				std::sort(assetItems.begin(), assetItems.end(), [](const auto& a, const auto& b) {
					if (a.type == Model::AssetType::Folder && b.type != Model::AssetType::Folder) return true;
					if (a.type != Model::AssetType::Folder && b.type == Model::AssetType::Folder) return false;
					return a.name < b.name;
					});

				if (ImGui::BeginPopupContextWindow("window_context", ImGuiPopupFlags_MouseButtonRight)) {
					if (ImGui::MenuItem("Add Folder")) {
						Service::AssetsService::Instance().addFolder("New Foldern", currentFolder);

					}
					if (ImGui::MenuItem("Add Texture")) {
						std::string texturePath = UI::openDialog();
						if (!texturePath.empty()) {
							Service::AssetsService::Instance().addTexture(texturePath, currentFolder);
						}
					}
					if (ImGui::MenuItem("Add Mesh")) {
						std::string meshPath = UI::openDialog();
						if (!meshPath.empty()) {
							Model::MeshData meshData = LoadEntity::LoadEntity().LoadMesh(meshPath);
							Service::AssetsService::Instance().SaveMesh(meshData, meshPath, currentFolder);
						}
					}
					if (ImGui::MenuItem("Add Script")) {
						//assetsService->addScript(currentFolder);
					}
					if (ImGui::MenuItem("Add Material")) {
						Service::AssetsService::Instance().addMaterial(currentFolder);
					}

					if (ImGui::MenuItem("Add HightMap")) {
						std::string texturePath = UI::openDialog();
						if (!texturePath.empty()) {
							Service::AssetsService::Instance().addHightMap(texturePath, currentFolder);
						}
					}


					ImGui::EndPopup();
				}

				ImVec2 iconSize(64, 64);
				float padding = 16.0f;
				float cellWidth = iconSize.x + padding;
				float availWidth = ImGui::GetContentRegionAvail().x;
				int columns = std::max(1, int(availWidth / cellWidth));

				for (int i = 0; i < assetItems.size(); ++i) {
					auto& item = assetItems[i];

					ImGui::PushID((int)i);
					ImGui::BeginGroup();

					if (assetItems[i].type == Model::AssetType::Folder)
					{
						ButtonIcon(i, Model::AssetType::Folder,
							[&]() {
								currentFolder = currentFolder + "/" + item.name;
							}
						);

					}
					if (assetItems[i].type == Model::AssetType::Material)
					{
						ButtonIcon(i, Model::AssetType::Material,
							[&]() {
								Service::EditorService::Instance().setMaterialSelected(&assetItems[i]);
							}
						);

					}
					if (assetItems[i].type == Model::AssetType::Mesh)
					{
						ButtonIcon(i, Model::AssetType::Mesh,
							[&]() {
								Service::EntityService enitityService;

								Entity::Entity* entity = enitityService.create(assetItems[i].path);

								if (Service::EditorService::Instance().GetSelectedEntity() == nullptr) {
									Service::EditorService::Instance().setSelectedEntity(entity);
								}

								AppContext::Instance().AddNewEntity(std::shared_ptr<Entity::Entity>(entity));
							}
						);
					}
					if (assetItems[i].type == Model::AssetType::Script)
					{
						ButtonIcon(i, Model::AssetType::Script,
							[&]() {
								std::cout << "Icon " << i << " clicked!" << std::endl;
							}
						);
					}
					if (assetItems[i].type == Model::AssetType::Texture)
					{

						UI::ImageButton((std::string("icon_") + std::to_string(i)).c_str(),
							(ImTextureID)(intptr_t)Material::TextureManager::GetAssets(assetItems[i].id), iconSize,
							[&]() {
								std::cout << "Icon " << i << " clicked!" << std::endl;
							}
						);


					}

					if (assetItems[i].type == Model::AssetType::HightMap)
					{

						UI::ImageButton((std::string("icon_") + std::to_string(i)).c_str(),
							(ImTextureID)(intptr_t)Material::TextureManager::Get(assetItems[i].id), iconSize,
							[&]() {
								AppContext::Instance().GetCurrentWorld().SetTerrainPath(assetItems[i].path);
							}
						);


					}


					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("%s", item.name.c_str());

					if (ImGui::BeginPopupContextItem("asset_context")) {
						if (ImGui::MenuItem("Rename")) {
							//UI::StartRenaming(i, item.name);
						}
						if (ImGui::MenuItem("Delete")) {
							//assetsService.removeAssetById(item.id);
						}
						ImGui::EndPopup();
					}
					else {
						std::string label = item.name;
						const float maxTextWidth = iconSize.x;

						if (ImGui::CalcTextSize(label.c_str()).x > maxTextWidth) {
							while (label.size() > 3 && ImGui::CalcTextSize((label + "").c_str()).x > maxTextWidth) {
								label.pop_back();
							}

						}
						ImGui::TextUnformatted(label.c_str());
					}

					ImGui::EndGroup();

					if ((i + 1) % columns != 0)
						ImGui::SameLine();

					ImGui::PopID();
				}

			}
		);
	}

	void RenderBreadcrumb(const std::string& currentPath, const std::function<void(const std::string&)>& onNavigate) {
		std::vector<std::string> parts;
		std::stringstream ss(currentPath);
		std::string item;

		while (std::getline(ss, item, '/')) {
			if (!item.empty()) {
				parts.push_back(item);
			}
		}

		std::filesystem::path pathAccum;

		for (size_t i = 0; i < parts.size(); ++i) {
			if (i > 0) ImGui::SameLine();

			pathAccum /= parts[i];

			if (ImGui::SmallButton((parts[i] + "##" + std::to_string(i)).c_str())) {
				onNavigate(pathAccum.generic_string());
			}

			if (i < parts.size() - 1) {
				ImGui::SameLine();
				ImGui::Text(">");
				ImGui::SameLine();
			}
		}
	}

	void ButtonIcon(int i, Model::AssetType type, const std::function<void()>& onClick) {
		ImVec2 iconSize(64, 64);
		GLuint icon = 0;
		switch (type) {
		case Model::AssetType::Folder:
			icon = icons[3];
			break;
		case Model::AssetType::Mesh:
			icon = icons[0];
			break;
		case Model::AssetType::Texture:
			icon = icons[1];
			break;
		case Model::AssetType::Script:
			icon = icons[2];
			break;
		case Model::AssetType::Material:
			icon = icons[4];
			break;
		default:
			break;
		}

		UI::ImageButton((std::string("icon_") + std::to_string(i)).c_str(),
			(ImTextureID)(intptr_t)icon, iconSize,
			[&]() {
				if (onClick) onClick();
			}
		);
	}

	void addIcon() {
		icons[0] = UI::LoadTextureFromFile("assets/geometry.png");
		icons[1] = UI::LoadTextureFromFile("assets/image.png");
		icons[2] = UI::LoadTextureFromFile("assets/script.png");
		icons[3] = UI::LoadTextureFromFile("assets/folder.png");
		icons[4] = UI::LoadTextureFromFile("assets/material.png");
	}


private:
	GLuint icons[5];
	std::string currentFolder = "Assets";
	MainWindow* host = nullptr;
};

#endif //ASSETSWINDOW_HPP
