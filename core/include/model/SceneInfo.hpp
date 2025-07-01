//
// Created by arunc on 01/07/2025.
//

#ifndef SCENEINFO_HPP
#define SCENEINFO_HPP
#include <string>
#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Model {

    class SceneInfo {
    public:
        std::string name;
        std::string path;
        std::chrono::system_clock::time_point createdAt;
        std::chrono::system_clock::time_point lastUpdated;

        SceneInfo() = default;

        SceneInfo(const std::string& n, const std::string& p)
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
            return pt;
        }


        static SceneInfo fromPtree(const boost::property_tree::ptree& pt) {
            SceneInfo info;
            info.name = pt.get<std::string>("Name");
            info.path = pt.get<std::string>("Path");
            info.createdAt = stringToTime(pt.get<std::string>("CreatedAt"));
            info.lastUpdated = stringToTime(pt.get<std::string>("LastUpdated"));
            return info;
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

}
#endif //SCENEINFO_HPP
