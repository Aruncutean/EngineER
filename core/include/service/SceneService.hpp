//
// Created by arunc on 01/07/2025.
//

#ifndef SCENESERVICE_HPP
#define SCENESERVICE_HPP
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "model/SceneInfo.hpp"
#include "AppContext.hpp"
#include "model/SceneData.hpp"
#include "io/SceneSerializer.hpp"

namespace Service {

	class  SceneService{
	public:
		SceneService()
		{
			const auto& project = AppContext::Instance().GetCurrentProject();
			projectDir = std::filesystem::path(project.path);
			projectJsonPath = projectDir / "project.json";
			scenesDir = projectDir / "Scenes";

			loadSceneList();
		}

		const std::vector<Model::SceneInfo>& getScenes() const {
			return scenes;
		}

		bool createScene(const std::string& name) {
			std::filesystem::path scenePath = scenesDir / (name + ".json");

			// Verifică dacă fișierul deja există
			if (std::filesystem::exists(scenePath)) {
				std::cerr << "Scena \"" << name << "\" deja există." << std::endl;
				return false;
			}

			// 1. Creează info și adaugă în lista de scene
			Model::SceneInfo info(name, scenePath.string());
			scenes.push_back(info);

			// 2. Actualizează și salvează project.json
			saveSceneList();

			// 3. Creează fișierul .json al scenei pe disc
			World* world = new World();
			world->SetName(name);
			world->SetPath(scenePath.string());
			SceneSerializer scene;
			scene.SaveScene(scenePath.string(), *world);

			/*Model::SceneData data(name, scenePath.string());
			boost::property_tree::ptree pt = data.toPtree();

			try {
				boost::property_tree::write_json(scenePath.string(), pt);
			}
			catch (const std::exception& e) {
				std::cerr << "Eroare la scrierea fișierului scenei: " << e.what() << std::endl;
				return false;
			}*/

			return true;
		}

		bool deleteScene(size_t index) {
			if (index >= scenes.size()) return false;

			std::filesystem::remove(scenes[index].path);
			scenes.erase(scenes.begin() + index);
			saveSceneList();

			return true;
		}

		Model::SceneData getSceneData(const std::string& sceneName) {
			std::filesystem::path scenePath = scenesDir / (sceneName + ".json");

			if (!std::filesystem::exists(scenePath)) {
				throw std::runtime_error("Fisierul scenei nu exista: " + scenePath.string());
			}

			boost::property_tree::ptree pt;
			boost::property_tree::read_json(scenePath.string(), pt);

			return Model::SceneData::fromPtree(pt);
		}

	private:
		std::filesystem::path projectJsonPath;
		std::filesystem::path projectDir;
		std::filesystem::path scenesDir;
		std::vector<Model::SceneInfo> scenes;

		void loadSceneList() {
			scenes.clear();
			boost::property_tree::ptree root;
			const auto& project = AppContext::Instance().GetCurrentProject();

			scenes = project.scenes;
		}

		void saveSceneList() {
			auto project = AppContext::Instance().GetCurrentProject();
			project.scenes = scenes;
			AppContext::Instance().SetCurrentProject(project);


			boost::property_tree::ptree root = project.toPtree();
			std::ofstream ofs(projectJsonPath, std::ios::trunc);
			boost::property_tree::write_json(ofs, root);
			ofs.close();
		}
	};

}

#endif //SCENESERVICE_HPP
