//
// Created by arunc on 03/07/2025.
//

#ifndef SCENEVIEW_HPP
#define SCENEVIEW_HPP
//
// Created by arunc on 03/07/2025.
//


#include "component/Button.hpp"
#include "component/Component.hpp"
#include "component/InputText.hpp"
#include "component/Label.hpp"
#include "component/List.hpp"
#include "component/SameLine.hpp"
#include "component/View.hpp"
#include "editor/EditorMenu.hpp"
#include "editor/EngineView.hpp"
#include "editor/InitEditor.hpp"
#include "service/ProjectService.hpp"
#include "service/SceneService.hpp"

class SceneView : public View {
public:
    SceneView(): View("SceneView") {
        noBackground = true;
        noCollapse = true;
        noMove = true;
        noResize = true;
        noTitleBar = true;
        useViewportLayout = true;
        setWindowStyle = true;

        service = new Service::SceneService();
        auto nameRow = std::make_shared<component::SameLine>();

        auto labelName = std::make_shared<component::Label>("Name");
        auto sceneInput = std::make_shared<component::InputText>("##SceneName");
        sceneInput->SetOnChangeCallback([this](const std::string &val) {
            sceneName = val;
        });

        auto sceneList = std::make_shared<component::List<Model::SceneInfo> >("Assets");
        auto saveBtn = std::make_shared<component::Button>("Save Scene");
        saveBtn->SetCallback([this, sceneList, sceneInput]() {
            if (!sceneName.empty() && service->createScene(sceneName)) {
                sceneList->AddItem(service->getScenes().at(service->getScenes().size() - 1));
                sceneInput->SetValue("");
                sceneName.clear();
            }
        });

        nameRow->AddComponent(labelName);
        nameRow->AddComponent(sceneInput);
        nameRow->AddComponent(saveBtn);


        sceneList->SetItems(service->getScenes());

        sceneList->SetToString([](const Model::SceneInfo &scene) {
            return scene.name;
        });

        sceneList->SetOnSelect([this](const Model::SceneInfo &selected) {
            auto data = service->getSceneData(selected.name);
            AppContext::Instance().SetCurrentScene(data);

            new InitEditor();
            service_editor::ViewService::Instance().RemoveView(shared_from_this());
        });

        sceneList->GetContextMenu().AddItem("Remove", [this](const Model::SceneInfo &selected) {
            // TODO: Remove logic
        });

        addComponent(nameRow);
        addComponent(sceneList);
    }

    ~SceneView() {
        std::cout << "SceneView" << std::endl;
    }

private:
    Service::SceneService *service = nullptr;

    std::string sceneName;
    std::string selectedSceneName;
};


#endif //SCENEVIEW_HPP
