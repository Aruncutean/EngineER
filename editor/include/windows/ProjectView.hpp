//
// Created by arunc on 03/07/2025.
//

#ifndef PROJECTVIEW_HPP
#define PROJECTVIEW_HPP

#include "SceneView.hpp"
#include "component/Button.hpp"
#include "component/Component.hpp"
#include "component/InputText.hpp"
#include "component/Label.hpp"
#include "component/List.hpp"
#include "component/SameLine.hpp"
#include "component/View.hpp"
#include "service/ProjectService.hpp"

class ProjectView : public View {
public:
    ProjectView(): View("ProjectView") {
        noBackground = true;
        noCollapse = true;
        noMove = true;
        noResize = true;
        noTitleBar = true;
        useViewportLayout = true;
        setWindowStyle = true;

        service = std::make_unique<Service::ProjectService>("projects.json");

        auto labelName = std::make_shared<component::Label>("Name");
        nameInput = std::make_shared<component::InputText>("Project Name");
        nameInput->SetOnChangeCallback([this](const std::string &val) {
            projectName = val;
        });

        auto nameRow = std::make_shared<component::SameLine>();
        nameRow->AddComponent(labelName);
        nameRow->AddComponent(nameInput);

        auto labelPath = std::make_shared<component::Label>("Path");
        pathInput = std::make_shared<component::InputText>("##Path");
        pathInput->disabled = true;
        pathInput->SetOnChangeCallback([this](const std::string &val) {
            path = val;
        });

        auto pathRow = std::make_shared<component::SameLine>();
        pathRow->AddComponent(labelPath);
        pathRow->AddComponent(pathInput);
        auto pathBtn = std::make_shared<component::Button>("Add Path");
        pathBtn->SetCallback([this]() {
            std::string folder = UI::openFolderDialog();
            if (!folder.empty()) {
                path = folder;
                pathInput->SetValue(folder);
            }
        });
        pathRow->AddComponent(pathBtn);

        projectList = std::make_shared<component::List<Model::ProjectInfo> >("Projects");
        RefreshProjectList();

        projectList->SetToString([](const Model::ProjectInfo &proj) {
            return proj.name;
        });

        projectList->SetOnSelect([this](const Model::ProjectInfo &proj) {
            selectedProject = proj;

            Model::ProjectData projectData = service->LoadFullProjectData(proj.path);
            AppContext::Instance().SetCurrentProject(projectData);

            service_editor::ViewService::Instance().AddView(std::make_shared<SceneView>());
            auto self = shared_from_this();
            service_editor::ViewService::Instance().RemoveView(self);
        });

        projectList->GetContextMenu().AddItem("Remove", [this](const Model::ProjectInfo &proj) {
            // service->removeProjectByName(proj.name);
            // selectedProject = {};
            // RefreshProjectList();
        });

        auto createBtn = std::make_shared<component::Button>("New Project");
        createBtn->SetCallback([this]() {
            if (projectName.empty() || path.empty()) {
                std::cerr << "Project name or path cannot be empty.\n";
                return;
            }

            if (service->createProject(projectName, path)) {
                AppContext::Instance().SetCurrentProject(service->LoadFullProjectData(path));
                service_editor::ViewService::Instance().AddView(std::make_shared<SceneView>());
                auto self = shared_from_this();
                service_editor::ViewService::Instance().RemoveView(self);
            }
        });

        addComponent(nameRow);
        addComponent(pathRow);
        addComponent(projectList);
        addComponent(createBtn);
    }


    ~ProjectView() {
        std::cout << "ProjectView" << std::endl;
    }

private:
    void RefreshProjectList() {
        const auto &projs = service->getProjects();
        projectList->SetItems(projs);
    }

    std::string projectName;
    std::string path;
    Model::ProjectInfo selectedProject;

    std::unique_ptr<Service::ProjectService> service;


    std::shared_ptr<component::InputText> nameInput;
    std::shared_ptr<component::InputText> pathInput;
    std::shared_ptr<component::List<Model::ProjectInfo> > projectList;
};

#endif //PROJECTVIEW_HPP
