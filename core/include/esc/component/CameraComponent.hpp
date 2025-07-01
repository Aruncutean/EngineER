//
// Created by arunc on 01/07/2025.
//

#ifndef CAMERACOMPONENT_HPP
#define CAMERACOMPONENT_HPP


#include <glm.hpp>
#include "ComponentBase.hpp"

namespace Component {

    class CameraComponent : public ComponentBase {
    public:
        float fieldOfView = 45.0f;
        float nearClip = 0.1f;
        float farClip = 1000.0f;

        glm::vec3 front = glm::vec3(0.0f, -0.5f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        bool isMainCamera = false;

        CameraComponent() = default;

        CameraComponent(const boost::property_tree::ptree& pt) {
            fieldOfView = pt.get<float>("FieldOfView", 45.0f);
            nearClip = pt.get<float>("NearClip", 0.1f);
            farClip = pt.get<float>("FarClip", 1000.0f);
            front = glm::vec3(
                pt.get<float>("FrontX", 0.0f),
                pt.get<float>("FrontY", -0.5f),
                pt.get<float>("FrontZ", -1.0f)
            );
            up = glm::vec3(
                pt.get<float>("UpX", 0.0f),
                pt.get<float>("UpY", 1.0f),
                pt.get<float>("UpZ", 0.0f)
            );
            isMainCamera = pt.get<bool>("IsMainCamera", false);
        }

        std::string GetTypeName() const override {
            return "CameraComponent";
        }

        boost::property_tree::ptree ToPtree() const override {
            boost::property_tree::ptree pt;
            pt.put("FieldOfView", fieldOfView);
            pt.put("NearClip", nearClip);
            pt.put("FarClip", farClip);
            pt.put("FrontX", front.x);
            pt.put("FrontY", front.y);
            pt.put("FrontZ", front.z);
            pt.put("UpX", up.x);
            pt.put("UpY", up.y);
            pt.put("UpZ", up.z);
            pt.put("IsMainCamera", isMainCamera);
            return pt;
        }

        void FromPtree(const boost::property_tree::ptree& pt) override {
            fieldOfView = pt.get<float>("FieldOfView", 45.0f);
            nearClip = pt.get<float>("NearClip", 0.1f);
            farClip = pt.get<float>("FarClip", 1000.0f);
            front = glm::vec3(
                pt.get<float>("FrontX", 0.0f),
                pt.get<float>("FrontY", -0.5f),
                pt.get<float>("FrontZ", -1.0f)
            );
            up = glm::vec3(
                pt.get<float>("UpX", 0.0f),
                pt.get<float>("UpY", 1.0f),
                pt.get<float>("UpZ", 0.0f)
            );
            isMainCamera = pt.get<bool>("IsMainCamera", false);
        }
    };

}

#endif //CAMERACOMPONENT_HPP
