//
// Created by arunc on 01/07/2025.
//

#ifndef HEIGHTMAPCOMPONENT_HPP
#define HEIGHTMAPCOMPONENT_HPP
#include "ComponentBase.hpp"
#include <string>
#include <glm.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Component {

    class HeightMapComponent : public ComponentBase {
    public:
        std::string imagePath;
        float scaleXZ = 1.0f;
        float scaleY = 10.0f;

        HeightMapComponent() = default;

        HeightMapComponent(const std::string& path, float scaleXZ, float scaleY)
            : imagePath(path), scaleXZ(scaleXZ), scaleY(scaleY) {
        }

        std::string GetTypeName() const override {
            return "HeightMapComponent";
        }

        boost::property_tree::ptree ToPtree() const override {
            boost::property_tree::ptree pt;
            pt.put("ImagePath", imagePath);
            pt.put("ScaleXZ", scaleXZ);
            pt.put("ScaleY", scaleY);
            return pt;
        }

        void FromPtree(const boost::property_tree::ptree& pt) override {
            imagePath = pt.get<std::string>("ImagePath", "");
            scaleXZ = pt.get<float>("scaleXZ", 1.0f);
            scaleY = pt.get<float>("scaleY", 10.0f);

        }
    };
}

#endif //HEIGHTMAPCOMPONENT_HPP
