//
// Created by arunc on 01/07/2025.
//

#ifndef PROJECTWINDOW_HPP
#define PROJECTWINDOW_HPP
// This is a conceptual C++ translation of your C# ProjectWindow class using Silk.NET and ImGui
// Assumes you're using equivalent C++ bindings or wrappers for Silk.NET (or directly using SDL/GLFW, etc.)

#include <imgui.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <glad/glad.h>
#include "component/Component.hpp"
#include "Window.hpp"
#include "service/ProjectService.hpp"
 #include "service/AppContext.hpp"
#include "MainWindow.hpp"
#include "SceneWindow.hpp"

class ProjectWindow : public Window, public std::enable_shared_from_this<ProjectWindow> {

public:

	ProjectWindow(MainWindow* host) : host(host) {
		service = new Service::ProjectService("projects.json");
	}

	ProjectWindow() {
		service = new Service::ProjectService("projects.json");
	}

	~ProjectWindow() {
		if (service) {
			delete service;
			service = nullptr;
		}
	}

	void Render() override {
		bool isOpen = true;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		//ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;


		UI::Window("Project", isOpen, flags, [&]() {

			ImGui::PopStyleVar(2);

			{
				UI::Text("Name");
				ImGui::SameLine();
				UI::InputText("##ProjectName", projectName, 256, true);
			}
			{
				UI::Text("Path");
				ImGui::SameLine();
				UI::InputText("##Path", path, 1024, false);
				ImGui::SameLine();
				UI::Button("Add Path", [&]()
					{
						std::string path = UI::openFolderDialog();
						if (!path.empty())
						{
							this->path = path;
						}
					});
			}
			{
				std::vector<Model::ProjectInfo>& projects = const_cast<std::vector<Model::ProjectInfo>&>(service->getProjects());

				UI::ListBoxWithEvents(
					"Projects",
					[&projects]()
					{
						std::vector<std::string> names;
						for (const auto& p : projects)
							names.push_back(p.name);
						return names;
					}(),
						selectedProjectIndex,
						[&](int index, const std::string& name)
						{
							selectedProjectIndex = index;
						},
						[&](int index, const std::string& name)
						{
							if (index >= 0 && index < projects.size())
							{

								Model::ProjectData projectData = service->LoadFullProjectData(service->getProjects().at(index).path);

								AppContext::Instance().SetCurrentProject(projectData);
							    host->AddWindow(std::make_shared<SceneWindow>(host));
								host->RemoveWindow(shared_from_this());

							}
						},
						[&](int index, const std::string& name)
						{
							if (ImGui::BeginPopupContextItem())
							{
								if (ImGui::MenuItem("Remove"))
								{
									if (index >= 0 && index < projects.size())
									{
										service->removeProject(index);
										if (selectedProjectIndex >= projects.size())
										{
											selectedProjectIndex = -1;
										}
									}
								}
								ImGui::EndPopup();
							}
						},
						10
						);
			}

			UI::Button("New Project", [&]()
				{
					if (projectName.empty() || path.empty())
					{
						std::cerr << "Project name or path cannot be empty." << std::endl;
						return;
					}

					if (service->createProject(projectName, path) == true)
					{
						host->AddWindow(std::make_shared<SceneWindow>(host));
						host->RemoveWindow(shared_from_this());
					}

				});
			});

	};

private:
	Service::ProjectService* service = nullptr;

	std::string projectName = "";
	std::string path = "";
	bool enableFeature = false;

	int selectedProjectIndex = -1;

	MainWindow* host;

};

#endif //PROJECTWINDOW_HPP
