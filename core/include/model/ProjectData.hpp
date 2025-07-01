//
// Created by arunc on 01/07/2025.
//

#ifndef PROJECTDATA_HPP
#define PROJECTDATA_HPP

#include <string>
#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "SceneInfo.hpp"

namespace Model {

	class ProjectData {
	public:
		std::string name;
		std::string path;
		std::chrono::system_clock::time_point createdAt;
		std::chrono::system_clock::time_point lastUpdated;
		std::vector<SceneInfo> scenes;

		ProjectData() = default;

		ProjectData(const std::string& n, const std::string& p)
			: name(n), path(p),
			createdAt(std::chrono::system_clock::now()),
			lastUpdated(std::chrono::system_clock::now()) {
		}

		boost::property_tree::ptree toPtree() const {
			boost::property_tree::ptree pt;
			pt.put("Name", name);
			pt.put("Path", path);
			pt.put("CreatedAt", timeToString(createdAt));
			pt.put("LastUpdated", timeToString(lastUpdated));

			boost::property_tree::ptree scenesNode;
			for (const auto& scene : scenes) {
				scenesNode.push_back(std::make_pair("", scene.toPtree()));
			}
			pt.add_child("Scenes", scenesNode);

			return pt;
		}


		static ProjectData fromPtree(const boost::property_tree::ptree& pt) {
			ProjectData data;
			data.name = pt.get<std::string>("Name");
			data.path = pt.get<std::string>("Path");
			data.createdAt = stringToTime(pt.get<std::string>("CreatedAt"));
			data.lastUpdated = stringToTime(pt.get<std::string>("LastUpdated"));

			if (auto scenesNode = pt.get_child_optional("Scenes")) {
				for (const auto& s : *scenesNode) {
					data.scenes.push_back(SceneInfo::fromPtree(s.second));
				}
			}

			return data;
		}

	private:
		static std::string timeToString(const std::chrono::system_clock::time_point& tp) {
			std::time_t t = std::chrono::system_clock::to_time_t(tp);
			char buffer[64];
			std::tm tm;
			localtime_s(&tm, &t);
			std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
			return buffer;
		}

		static std::chrono::system_clock::time_point stringToTime(const std::string& s) {
			std::tm tm = {};
			std::istringstream ss(s);
			ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
			std::time_t time = std::mktime(&tm);
			return std::chrono::system_clock::from_time_t(time);
		}
	};

}
#endif //PROJECTDATA_HPP
