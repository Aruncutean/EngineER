//
// Created by arunc on 01/07/2025.
//

#ifndef MATERIALMANAGER_HPP
#define MATERIALMANAGER_HPP
#include <unordered_map>
#include <string>
#include <memory>
#include <filesystem>
#include <optional>
#include "model/AssetItem.hpp"
#include "service/AppContext.hpp"
#include "io/MaterialIO.hpp"


namespace Material {

	class MaterialManager {
	public:
		static std::shared_ptr<MaterialBase> Get(const std::string& id) {
			if (id == "default") {
				auto defaultMaterial = std::make_shared<MaterialDefault>();
				defaultMaterial->diffuseColor[0] = 1.0f;
				defaultMaterial->diffuseColor[1] = 1.0f;
				defaultMaterial->diffuseColor[2] = 1.0f;
				defaultMaterial->specularColor[0] = 1.0f;
				defaultMaterial->specularColor[1] = 1.0f;
				defaultMaterial->specularColor[2] = 1.0f;
				defaultMaterial->shininess = 32.0f;
				return std::static_pointer_cast<MaterialBase>(defaultMaterial);
			}

			auto it = materials.find(id);
			if (it != materials.end()) {
				return it->second;
			}

			std::optional<Model::ProjectData> projectData = AppContext::Instance().GetCurrentProject();
			if (projectData.has_value()) {
				std::filesystem::path assetPath = std::filesystem::path(projectData->path) / "Assets" / "assets.json";
				if (!std::filesystem::exists(assetPath)) {
					std::cerr << "Asset file not found: " << assetPath.string() << std::endl;
					return nullptr;
				}

				auto assetCollection = new std::vector<Model::AssetItem>();
				boost::property_tree::ptree root;
				try {
					boost::property_tree::read_json(assetPath.string(), root);
					for (const auto& item : root.get_child("Assets")) {
						assetCollection->push_back(Model::AssetItem::fromPtree(item.second));
					}
				}
				catch (const std::exception& e) {
					std::cerr << "Error reading asset.json: " << e.what() << std::endl;
				}

				auto itAsset = std::find_if(assetCollection->begin(), assetCollection->end(),
					[&id](const Model::AssetItem& asset) {
						return asset.id == id;
					});

				if (itAsset != assetCollection->end()) {
					std::filesystem::path materialPath = std::filesystem::path(projectData->path) / itAsset->path;
					MaterialIO materialIO;
					auto material = materialIO.Load(materialPath.string());
					materials[id] = material;
					return material;
				}
			}

			return nullptr;
		}

		static void Register(const std::string& id, std::shared_ptr<MaterialBase> material) {
			materials[id] = material;
		}


	private:
		static inline std::unordered_map<std::string, std::shared_ptr<MaterialBase>> materials;
	};

}
#endif //MATERIALMANAGER_HPP
