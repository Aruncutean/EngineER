//
// Created by arunc on 02/07/2025.
//
#include "component/View.hpp"
#include "service/ViewService.hpp"

void View::render() {
    auto self = shared_from_this();
    if (!isOpen) {
        service_editor::ViewService::Instance().RemoveView(self);
        return;
    }

    ImGuiWindowFlags flags = 0;

    if (noTitleBar) flags |= ImGuiWindowFlags_NoTitleBar;
    if (noResize) flags |= ImGuiWindowFlags_NoResize;
    if (noMove) flags |= ImGuiWindowFlags_NoMove;
    if (noScrollbar) flags |= ImGuiWindowFlags_NoScrollbar;
    if (noCollapse) flags |= ImGuiWindowFlags_NoCollapse;
    if (noBackground) flags |= ImGuiWindowFlags_NoBackground;
    if (alwaysAutoResize) flags |=    ImGuiWindowFlags_AlwaysAutoResize
;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.18f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.0f);



    if (useViewportLayout) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
    }

    if (setWindowStyle) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    }


    ImGui::Begin(name.c_str(), &isOpen, flags);

    ImGui::PopStyleVar(2);

    if (enableDockspace) {
        ImGuiID dockspace_id = ImGui::GetID((name + "_Dockspace").c_str());
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));
    }

    for (auto &component: components) {
        if(component!= nullptr)
        component->render();
    }

    ImGui::End();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}