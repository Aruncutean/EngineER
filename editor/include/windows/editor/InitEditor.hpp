//
// Created by arunc on 04/07/2025.
//

#ifndef INITEDITOR_HPP
#define INITEDITOR_HPP
#include "AssetsView.hpp"
#include "CameraInfoView.hpp"
#include "EditorMenu.hpp"
#include "EditorView.hpp"
#include "EngineView.hpp"
#include "SceneHierarchyView.hpp"
#include "SceneRenderView.hpp"

class InitEditor {
public:
    void RegisterAllComponents() {
        using namespace Component;
        auto& factory = ComponentFactory::Instance();
        factory.Register<TransformComponent>("TransformComponent");
        factory.Register<MaterialComponent>("MaterialComponent");
        factory.Register<MeshComponent>("MeshComponent");
        factory.Register<CameraComponent>("CameraComponent");
        factory.Register<LightComponent>("LightComponent");
        factory.Register<PhysicsComponent>("PhysicsComponent");
    }


    InitEditor() {
        WindowService::getInstance()->IsBockingSpace = true;
        RegisterAllComponents();
        new EditorMenu();
        service_editor::ViewService::Instance().AddView(std::make_shared<EngineView>());
        service_editor::ViewService::Instance().AddView(std::make_shared<EditorView>());
        service_editor::ViewService::Instance().AddView(std::make_shared<AssetsView>());
        service_editor::ViewService::Instance().AddView(std::make_shared<SceneRenderView>());
        service_editor::ViewService::Instance().AddView(std::make_shared<CameraInfoView>());
        service_editor::ViewService::Instance().AddView(std::make_shared<SceneHierarchyView>());

    }

    ~InitEditor() {
        std::cout << "InitEditor" << std::endl;
    }
};

#endif //INITEDITOR_HPP
