//
// Created by arunc on 01/07/2025.
//

#ifndef SCENESERIALIZER_HPP
#define SCENESERIALIZER_HPP
#include <string>
#include "model/World.hpp"

using namespace boost::property_tree;

class SceneSerializer {
public:
	static void SaveScene(const std::string& path, const World& scene) {
		using namespace boost::property_tree;

		ptree root;

		ptree entitiesArray;

		for (const auto& entity : scene.GetEntities()) {
			ptree entityNode = entity->ToPtree();
			entitiesArray.push_back(std::make_pair("", entityNode));
		}

		root.put("Name", scene.GetName());
		root.put("Path", scene.GetPath());
		root.put("CreatedAt", timeToString(scene.GetCreatedAt()));
		root.put("LastUpdated", timeToString(scene.GetLastUpdated()));
		root.put("TerrainPath", scene.GeTerrainPath());
		if (!entitiesArray.empty()) {
			root.add_child("Entities", entitiesArray);
		}
		write_json(path, root);
	}


	static  World LoadScene(const std::string& path)
	{
		ptree root;
		read_json(path, root);

		World scene;
		scene.SetName(root.get<std::string>("Name", ""));
		scene.SetPath(root.get<std::string>("Path", ""));
		scene.SetTerrainPath(root.get<std::string>("TerrainPath", ""));
		auto createdAt = stringToTime(root.get<std::string>("CreatedAt"));
		auto lastUpdated = stringToTime(root.get<std::string>("LastUpdated"));

		if (root.find("Entities") != root.not_found()) {
			for (const auto& [_, entityNode] : root.get_child("Entities")) {
				auto entity = std::make_shared<Entity::Entity>();
				entity->FromPtree(entityNode);
				scene.AddEntity(entity);
			}
		}
		else {
			std::cerr << "Warning: 'entities' not found in JSON." << std::endl;
		}

		return scene;
	}


private:
	static std::string timeToString(const std::chrono::system_clock::time_point& tp) {
		std::time_t t = std::chrono::system_clock::to_time_t(tp);
		char buffer[64];
		std::tm tm;
		localtime_s(&tm, &t);
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &tm);
		return buffer;
	}

	static std::chrono::system_clock::time_point stringToTime(const std::string& s) {
		std::tm tm = {};
		std::istringstream ss(s);
		ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
		std::time_t time = std::mktime(&tm);
		return std::chrono::system_clock::from_time_t(time);
	}
};
#endif //SCENESERIALIZER_HPP
