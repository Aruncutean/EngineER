//
// Created by arunc on 01/07/2025.
//

#ifndef CAMERACONTROLLERCOMPONENT_HPP
#define CAMERACONTROLLERCOMPONENT_HPP
#include <glm.hpp>
#include "ComponentBase.hpp"

namespace Component {

    class CameraControllerComponent : public ComponentBase {
    public:
        float MoveSpeed = 30.0f;
        float LookSensitivity = 0.1f;

        CameraControllerComponent() = default;




        std::string GetTypeName() const override {
            return "CameraControllerComponent";
        }

        boost::property_tree::ptree ToPtree() const override {
            boost::property_tree::ptree pt;
            pt.put("MoveSpeed", MoveSpeed);
            pt.put("LookSensitivity", LookSensitivity);

            return pt;
        }

        void FromPtree(const boost::property_tree::ptree& pt) override {
            MoveSpeed = pt.get<float>("MoveSpeed", 5);
            LookSensitivity = pt.get<float>("LookSensitivity", 0.1f);
        }

    };

}

#endif //CAMERACONTROLLERCOMPONENT_HPP
