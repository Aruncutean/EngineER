//
// Created by arunc on 07/07/2025.
//

#ifndef CAMERAINFOVIEW_HPP
#define CAMERAINFOVIEW_HPP
#include "component/DragFloat.hpp"
#include "component/DragFloat3.hpp"
#include "component/Label.hpp"
#include "esc/component/CameraComponent.hpp"
#include "esc/component/TransformComponent.hpp"

class View;

class CameraInfoView : public View {
public:
    CameraInfoView(): View("CameraInfoView") {
        noCollapse = true;

        using namespace component;

        auto cameraEntity = Service::EditorService::Instance().getEditorCamera();
        if (!cameraEntity) return;

        if (!cameraEntity->HasComponent<Component::TransformComponent>() ||
            !cameraEntity->HasComponent<Component::CameraComponent>())
            return;

        auto transform = cameraEntity->GetComponent<Component::TransformComponent>();
        auto cameraC = cameraEntity->GetComponent<Component::CameraComponent>();

        // Transform Section
        addComponent(std::make_shared<Label>("Transform"));

        auto pos = std::make_shared<DragFloat3>("Position", &transform->position);
        glm::vec3 eulerAngles = glm::degrees(glm::eulerAngles(transform->rotation));
        auto rot = std::make_shared<DragFloat3>("Rotation", &eulerAngles);
        rot->SetOnChangeCallback([this, transform]( glm::vec3 *val) {
            transform->rotation =  glm::quat(glm::radians(*val));
        });
        addComponent(rot);
        auto scl = std::make_shared<DragFloat3>("Scale",&transform->scale);

        addComponent(pos);

        addComponent(scl);

        // Separator (optional visual, not a component yet)
        addComponent(std::make_shared<Label>(" "));

        // Camera Settings
        addComponent(std::make_shared<Label>("Camera"));

        auto fov = std::make_shared<DragFloat>("FOV", &cameraC->fieldOfView, 0.1f, 1.0f, 180.0f);
        auto nearClip = std::make_shared<DragFloat>("Near Clip", &cameraC->nearClip, 0.01f, 0.001f, 10.0f);
        auto farClip = std::make_shared<DragFloat>("Far Clip", &cameraC->farClip, 1.0f, 10.0f, 10000.0f);

        addComponent(fov);
        addComponent(nearClip);
        addComponent(farClip);

        // Separator
        addComponent(std::make_shared<Label>(" "));

        // Direction Vectors (read-only)
        addComponent(std::make_shared<Label>("Direction Vectors"));

        const glm::vec3 front = cameraC->front;
        const glm::vec3 up = cameraC->up;

        char buf[128];
        snprintf(buf, sizeof(buf), "Front: %.2f %.2f %.2f", front.x, front.y, front.z);
        addComponent(std::make_shared<Label>(buf));


        snprintf(buf, sizeof(buf), "Up: %.2f %.2f %.2f", up.x, up.y, up.z);
        addComponent(std::make_shared<Label>(buf));
    }
};


#endif //CAMERAINFOVIEW_HPP
