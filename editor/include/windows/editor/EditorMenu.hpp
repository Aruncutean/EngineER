//
// Created by arunc on 04/07/2025.
//

#ifndef EDITORMENU_HPP
#define EDITORMENU_HPP
#include "AssetsView.hpp"
#include "CameraInfoView.hpp"
#include "EditorView.hpp"
#include "EngineView.hpp"
#include "SceneRenderView.hpp"
#include "component/MainMenuBar.hpp"
#include "component/Menu.hpp"
#include "service/ViewService.hpp"
#include "component/IComponent.hpp"
#include <boost/process.hpp>
#include <iostream>
#include <string>

class EditorMenu {
public:
    EditorMenu() {
        auto fileMenu = std::make_shared<component::Menu>("File");
        fileMenu->AddItem("New", []() {
        });
        fileMenu->AddItem("Open", []() {
        });

        auto editMenu = std::make_shared<component::Menu>("Edit");
        editMenu->AddItem("Save", []() {
            std::string path = AppContext::Instance().GetCurrentScene().path;
            SceneSerializer::SaveScene(path, AppContext::Instance().GetCurrentWorld());
        });
        editMenu->AddItem("Undo", []() {
        });
        editMenu->AddItem("Redo", []() {
        });

        auto viewMenu = std::make_shared<component::Menu>("View");
        viewMenu->AddItem("Assets", []() {
            if (!service_editor::ViewService::Instance().GetViewByName("AssetsView")) {
                service_editor::ViewService::Instance().AddView(std::make_shared<AssetsView>());
            }
        });
        viewMenu->AddItem("Scene", []() {
            if (!service_editor::ViewService::Instance().GetViewByName("SceneRenderView")) {
                service_editor::ViewService::Instance().AddView(std::make_shared<SceneRenderView>());
            }
        });
        viewMenu->AddItem("Editor", []() {
            if (!service_editor::ViewService::Instance().GetViewByName("EditorView")) {
                service_editor::ViewService::Instance().AddView(std::make_shared<EditorView>());
            }
        });
        viewMenu->AddItem("Engine", []() {
            if (!service_editor::ViewService::Instance().GetViewByName("EngineView")) {
                service_editor::ViewService::Instance().AddView(std::make_shared<EngineView>());
            }
        });
        viewMenu->AddItem("Engine", []() {
            if (!service_editor::ViewService::Instance().GetViewByName("CameraInfoView")) {
                service_editor::ViewService::Instance().AddView(std::make_shared<CameraInfoView>());
            }
        });

        auto buildMenu = std::make_shared<component::Menu>("Build");
        buildMenu->AddItem("Build", []() {
        });
        buildMenu->AddItem("Run", [this]() {
            RunExternalApp("mytool.exe", {"--input", "file.txt", "--verbose"});
        });


        auto mainMenuBar = std::make_shared<component::MainMenuBar>();
        mainMenuBar->AddMenu(fileMenu);
        mainMenuBar->AddMenu(editMenu);
        mainMenuBar->AddMenu(viewMenu);

        service_editor::ViewService::Instance().AddComponent(mainMenuBar);
    }

    void RunExternalApp(const std::string &exePath, const std::vector<std::string> &args) {
        namespace bp = boost::process;
        bp::child process(exePath, bp::args(args));
        process.wait(); // așteaptă să termine
    }
};

#endif //EDITORMENU_HPP
