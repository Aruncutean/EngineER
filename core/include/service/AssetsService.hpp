//
// Created by arunc on 01/07/2025.
//

#ifndef ASSETSSERVICE_HPP
#define ASSETSSERVICE_HPP
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "model/AssetItem.hpp"
#include "AppContext.hpp"
#include "model/MeshData.hpp"
#include "MeshService.hpp"
#include "gui/material/Material.hpp"
#include "io/MaterialIO.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace Service {
	class AssetsService {
	public:
		static AssetsService& Instance() {
			static AssetsService instance;
			return instance;
		}

		const std::vector<Model::AssetItem>& getAssets() const {
			return assets;
		}

		void addAsset(const Model::AssetItem& item) {
			assets.push_back(item);
			save();
		}

		void addTexture(const std::string& sourcePath, const std::string& currentFolder) {
			namespace fs = std::filesystem;

			if (!fs::exists(sourcePath)) {
				std::cerr << "File not found: " << sourcePath << std::endl;
				return;
			}

			std::string fileName = fs::path(sourcePath).filename().string();


			Model::AssetItem asset;
			asset.id = generateRandomID();
			asset.name = fileName;
			asset.type = Model::AssetType::Texture;
			asset.baseDirectory = currentFolder;
			asset.path = currentFolder + "/" + fileName;

			addAsset(asset);

			std::string destinationDir = AppContext::Instance().GetCurrentProject().path + "/" + currentFolder;
			fs::create_directories(destinationDir);

			std::string destinationPath = destinationDir + "/" + fileName;
			fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);


		}


		void addHightMap(const std::string& sourcePath, const std::string& currentFolder) {
			namespace fs = std::filesystem;

			if (!fs::exists(sourcePath)) {
				std::cerr << "File not found: " << sourcePath << std::endl;
				return;
			}

			std::string fileName = fs::path(sourcePath).filename().string();


			Model::AssetItem asset;
			asset.id = generateRandomID();
			asset.name = fileName;
			asset.type = Model::AssetType::HightMap;
			asset.baseDirectory = currentFolder;
			asset.path = currentFolder + "/" + fileName;

			addAsset(asset);

			std::string destinationDir = AppContext::Instance().GetCurrentProject().path + "/" + currentFolder;
			fs::create_directories(destinationDir);

			std::string destinationPath = destinationDir + "/" + fileName;
			fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);


		}

		bool addFolder(const std::string& name, const std::string& currentFolder) {
			using namespace std::filesystem;

			std::string uniqueName = getUniqueFolderName(currentFolder, name);
			std::string relativePath = currentFolder + "/" + uniqueName;

			Model::AssetItem folder;
			folder.id = generateRandomID();
			folder.name = uniqueName;
			folder.type = Model::AssetType::Folder;
			folder.baseDirectory = currentFolder;
			folder.path = relativePath;
			folder.createdAt = std::chrono::system_clock::now();

			auto projectRoot = AppContext::Instance().GetCurrentProject().path;

			std::filesystem::path fullPath = std::filesystem::path(projectRoot) / relativePath;
			try {
				create_directories(fullPath.c_str());
			}
			catch (const std::exception& e) {
				std::cerr << "Eroare la crearea folderului: " << e.what() << std::endl;
				return false;
			}

			addAsset(folder);
			return true;
		}

		Model::AssetItem SaveMesh(const Model::MeshData& meshData, const std::string& importPath, const std::string& relativePath) {
			auto project = AppContext::Instance().GetCurrentProject();
			if (project.path.empty()) {
				throw std::runtime_error("Project path is not set.");
			}

			std::filesystem::path importFile(importPath);
			std::string baseName = importFile.stem().string();

			std::filesystem::path fullPath = std::filesystem::path(project.path) / relativePath;
			std::filesystem::create_directories(fullPath);

			std::string candidateName = baseName;
			int counter = 1;
			std::filesystem::path fullFilePath = fullPath / (candidateName + ".mesh.json");

			while (std::filesystem::exists(fullFilePath)) {
				candidateName = baseName + " (" + std::to_string(counter++) + ")";
				fullFilePath = fullPath / (candidateName + ".mesh.json");
			}

			Service::MeshService::Save(meshData, fullFilePath.string());

			Model::AssetItem asset;
			asset.id = generateRandomID();
			asset.name = candidateName;
			asset.path = (relativePath + "/" + candidateName);
			asset.baseDirectory = relativePath;
			asset.type = Model::AssetType::Mesh;
			asset.createdAt = std::chrono::system_clock::now();


			addAsset(asset);
			return asset;
		}

		std::string generateRandomID() {
			boost::uuids::uuid uuid = boost::uuids::random_generator()();
			return boost::uuids::to_string(uuid);
		}

		void addMaterial(const std::string& currentFolder) {
			using namespace std::filesystem;

			auto& project = AppContext::Instance().GetCurrentProject();
			std::string materialName = GetUniqueMaterialPath(currentFolder, "New Material");

			Model::AssetItem assetItem;
			assetItem.id = generateRandomID();
			assetItem.name = materialName;
			assetItem.type = Model::AssetType::Material;
			assetItem.baseDirectory = currentFolder;
			assetItem.path = currentFolder + "/" + materialName + ".material.json";

			std::shared_ptr<Material::MaterialBase> material = std::make_shared<Material::MaterialPhong>();

			material->id = assetItem.id;
			material->name = materialName;
			material->path = assetItem.path;
			std::filesystem::path fullFilePath = std::filesystem::path(project.path) / currentFolder / (materialName + ".material.json");
			MaterialIO materialIO;
			materialIO.Save(fullFilePath.string(), material);

			addAsset(assetItem);
		}

		std::string GetUniqueMaterialPath(const std::string& baseDirectory, const std::string& baseName) {
			namespace fs = std::filesystem;

			std::string name = baseName;
			std::string fileName = name + ".material.json";
			std::string projectPath = AppContext::Instance().GetCurrentProject().path;

			fs::path fullPath = fs::path(projectPath) / baseDirectory / fileName;

			int counter = 1;
			while (fs::exists(fullPath)) {
				name = baseName + " (" + std::to_string(counter) + ")";
				fileName = name + ".material.json";
				fullPath = fs::path(projectPath) / baseDirectory / fileName;
				++counter;
			}

			return name;
		}

		std::string getUniqueFolderName(const std::string& basePath, const std::string& baseName) {
			namespace fs = std::filesystem;

			const auto& project = AppContext::Instance().GetCurrentProject();
			fs::path projectRoot = project.path;

			std::string candidate = baseName;
			int counter = 1;

			fs::path candidatePath = projectRoot / basePath / candidate;

			while (fs::exists(candidatePath)) {
				candidate = baseName + " (" + std::to_string(counter++) + ")";
				candidatePath = projectRoot / basePath / candidate;
			}

			return candidate;
		}


		bool removeAssetById(const std::string& id) {
			auto it = std::remove_if(assets.begin(), assets.end(), [&](const auto& a) { return a.id == id; });
			if (it != assets.end()) {
				assets.erase(it, assets.end());
				save();
				return true;
			}
			return false;
		}

		bool save() {
			boost::property_tree::ptree root;
			boost::property_tree::ptree assetsNode;
			for (const auto& asset : assets) {
				assetsNode.push_back(std::make_pair("", asset.toPtree()));
			}
			root.add_child("Assets", assetsNode);
			try {
				std::ofstream ofs(assetFilePath);
				if (!ofs.is_open()) {
					std::cerr << "Failed to open file: " << assetFilePath << std::endl;
					return false;
				}
				boost::property_tree::write_json(ofs, root);
				return true;
			}
			catch (const std::exception& e) {
				std::cerr << "Error saving asset.json: " << e.what() << std::endl;
				return false;
			}
		}

	private:
		static std::vector<Model::AssetItem> assets;
		std::filesystem::path assetFilePath;
		std::filesystem::path projectJsonPath;
		std::filesystem::path projectDir;
		std::filesystem::path assetsDir;

		AssetsService()
		{
			const std::string& assetFile = "assets.json";
			const auto& project = AppContext::Instance().GetCurrentProject();
			projectDir = std::filesystem::path(project.path);
			assetsDir = projectDir / "Assets";
			assetFilePath = assetsDir / assetFile;

			load();
		}


		void load() {
			assets.clear();
			if (!std::filesystem::exists(assetFilePath))
				return;

			boost::property_tree::ptree root;
			try {
				boost::property_tree::read_json(assetFilePath.string(), root);
				for (const auto& item : root.get_child("Assets")) {
					assets.push_back(Model::AssetItem::fromPtree(item.second));
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Error reading asset.json: " << e.what() << std::endl;
			}
		}
	};
}

#endif //ASSETSSERVICE_HPP
