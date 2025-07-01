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
#include "editor/EditorWindow.hpp"

#include "service/SceneService.hpp"
#include "service/SceneService.hpp"

class SceneWindow : public Window, public std::enable_shared_from_this<SceneWindow> {

public:

	SceneWindow(MainWindow* host) : host(host) {
		service = new Service::SceneService();

	}

	SceneWindow() = default;

	~SceneWindow() {
	}

	void Render() override {
		bool isOpen = true;
		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		UI::Window("Scene", isOpen, flags, [&]() {
			ImGui::PopStyleVar(2);

			{
				UI::Text("Name");
				ImGui::SameLine();
				UI::InputText("##SceneName", sceneName, 256, true);
				ImGui::SameLine();
				UI::Button("Save Scene", [&]() {
					if (!sceneName.empty()) {
						if (service->createScene(sceneName))
						{
							sceneName.clear();
						}
					}
					}
				);
				std::vector<Model::SceneInfo>& sceneInfos = const_cast<std::vector<Model::SceneInfo>&>(service->getScenes());
				UI::ListBoxWithEvents(
					"Assets",
					[&sceneInfos]()
					{
						std::vector<std::string> names;
						for (const auto& p : sceneInfos)
							names.push_back(p.name);
						return names;
					}(),
						selectedAssetIndex,
						[&](int index, const std::string& name)
						{
							selectedAssetIndex = index;
						},
						[&](int index, const std::string& name)
						{
							if (index >= 0 && index < sceneInfos.size())
							{

								Model::SceneData data = service->getSceneData(sceneInfos[index].name);

								AppContext::Instance().SetCurrentScene(data);

								host->AddWindow(std::make_shared<EditorWindow>(host));
								host->RemoveWindow(shared_from_this());

							}
						},
						[&](int index, const std::string& name)
						{
							if (ImGui::BeginPopupContextItem())
							{
								if (ImGui::MenuItem("Remove"))
								{

								}
								ImGui::EndPopup();
							}
						},
						10
						);
			}

			});

	};

private:
	Service::SceneService* service = nullptr;
	std::string sceneName;

	int selectedAssetIndex = -1;
	MainWindow* host;
};
