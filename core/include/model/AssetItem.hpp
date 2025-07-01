//
// Created by arunc on 01/07/2025.
//

#ifndef ASSETITEM_HPP
#define ASSETITEM_HPP

#include <string>
#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <random>
#include <sstream>
#include <iomanip>

namespace Model {

	enum class AssetType {
		Mesh, Texture, Folder, Material, Script, HightMap
	};

	class AssetItem {
	public:
		std::string id;
		std::string name;
		std::string path;
		std::string baseDirectory;
		AssetType type = AssetType::Folder;
		std::chrono::system_clock::time_point createdAt;

		AssetItem() = default;

		AssetItem(const std::string& n, const std::string& p, AssetType t, const std::string& baseDir)
			: id(generateUUID()), name(n), path(p), baseDirectory(baseDir), type(t), createdAt(std::chrono::system_clock::now()) {
		}

		boost::property_tree::ptree toPtree() const {
			boost::property_tree::ptree pt;
			pt.put("id", id);
			pt.put("name", name);
			pt.put("path", path);
			pt.put("baseDirectory", baseDirectory);
			pt.put("type", static_cast<int>(type));
			pt.put("createdAt", timeToString(createdAt));
			return pt;
		}

		static AssetItem fromPtree(const boost::property_tree::ptree& pt) {
			AssetItem item;
			item.id = pt.get<std::string>("id");
			item.name = pt.get<std::string>("name");
			item.path = pt.get<std::string>("path");
			item.baseDirectory = pt.get<std::string>("baseDirectory");
			item.type = static_cast<AssetType>(pt.get<int>("type"));
			item.createdAt = stringToTime(pt.get<std::string>("createdAt"));
			return item;
		}

	private:
		static std::string generateUUID() {
			std::stringstream ss;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 15);
			std::uniform_int_distribution<> dis2(8, 11);

			ss << std::hex;
			for (int i = 0; i < 8; ++i) ss << dis(gen);
			ss << "-";
			for (int i = 0; i < 4; ++i) ss << dis(gen);
			ss << "-4";
			for (int i = 0; i < 3; ++i) ss << dis(gen);
			ss << "-";
			ss << dis2(gen);
			for (int i = 0; i < 3; ++i) ss << dis(gen);
			ss << "-";
			for (int i = 0; i < 12; ++i) ss << dis(gen);
			return ss.str();
		}

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

}

#endif //ASSETITEM_HPP
