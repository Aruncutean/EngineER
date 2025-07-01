//
// Created by arunc on 01/07/2025.
//

#ifndef PROJECTSERVICE_HPP
#define PROJECTSERVICE_HPP
#include <string>
#include <vector>
#include <functional>
#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "model/ProjectInfo.hpp"
#include <filesystem>
#include "AppContext.hpp"
#include "model/ProjectData.hpp"
#include <boost/process.hpp>

namespace Service {


	class ProjectService
	{
	public:

		ProjectService() = default;

		ProjectService(const std::string& filePath) : jsonFilePath(filePath) {
			loadFromFile();
		}

		const std::vector<Model::ProjectInfo>& getProjects() const {
			return projects;
		}

		void addProject(const Model::ProjectInfo& project) {
			projects.push_back(project);
			saveToFile();
		}

		void RunProject()
		{
			const std::string& projectPath = AppContext::Instance().GetCurrentProject().path;
			const std::string& scenePath = AppContext::Instance().GetCurrentScene().path;

			namespace bp = boost::process;

			std::string exePath = "C:\\Users\\arunc\\Desktop\\GameEditor\\RunTime\\bin\\Debug\\net8.0\\RunTime.exe";

			if (!std::filesystem::exists(exePath)) {
				std::cout << "Runtime exe not found.\n";
				return;
			}

			std::string args = "\"" + projectPath + "\" --edit --scene \"" + scenePath + "\"";

			try {
				bp::child c(exePath + " " + args);
				std::cout << "Launched process.\n";
				c.detach();
			}
			catch (std::exception& e) {
				std::cerr << "Failed to launch: " << e.what() << "\n";
			}
		}


		bool createProject(const std::string& name, const std::string& basePath) {
			namespace fs = std::filesystem;

			fs::path projectFolder = fs::path(basePath) / name;

			try {
				fs::create_directories(projectFolder / "Assets");
				fs::create_directories(projectFolder / "Scenes");
				fs::create_directories(projectFolder / "Scripts");

				Model::ProjectData data(name, projectFolder.string());
				AppContext::Instance().SetCurrentProject(data);

				boost::property_tree::ptree pt = data.toPtree();
				boost::property_tree::write_json((projectFolder / "project.json").string(), pt);


				Model::ProjectInfo info(name, projectFolder.string());
				addProject(info);
				return true;
			}
			catch (const std::exception& e) {
				std::cerr << "Eroare la crearea proiectului: " << e.what() << std::endl;
				return false;
			}
		}

		void saveToFile() {
			boost::property_tree::ptree root;
			boost::property_tree::ptree projectArray;

			for (const auto& proj : projects) {
				projectArray.push_back(std::make_pair("", proj.toPtree()));
			}

			root.add_child("projects", projectArray);
			boost::property_tree::write_json(jsonFilePath, root);
		}

		bool removeProject(size_t index) {
			if (index >= projects.size())
				return false;

			projects.erase(projects.begin() + index);
			saveToFile();
			return true;
		}


		void loadFromFile() {
			projects.clear();
			boost::property_tree::ptree root;

			try {
				boost::property_tree::read_json(jsonFilePath, root);
				for (const auto& item : root.get_child("projects")) {
					projects.push_back(Model::ProjectInfo::fromPtree(item.second));
				}
			}
			catch (...) {

			}
		}

		static Model::ProjectData LoadFullProjectData(const std::string& projectPath) {
			namespace fs = std::filesystem;

			fs::path projectFile = fs::path(projectPath) / "project.json";
			boost::property_tree::ptree pt;
			boost::property_tree::read_json(projectFile.string(), pt);

			return Model::ProjectData::fromPtree(pt);
		}

	private:
		std::string jsonFilePath;
		std::vector<Model::ProjectInfo> projects;

	};


}
#endif //PROJECTSERVICE_HPP
