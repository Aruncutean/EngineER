//
// Created by arunc on 02/07/2025.
//

#ifndef IMGUISTYLECONFIG_HPP
#define IMGUISTYLECONFIG_HPP

#include <imgui.h>

inline void ApplyCustomImGuiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.FramePadding = ImVec2(8, 6);
    style.WindowPadding = ImVec2(10, 10);
    style.ItemSpacing = ImVec2(10, 8);
    style.WindowMinSize = ImVec2(300, 80);

    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg]        = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_ChildBg]         = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
    colors[ImGuiCol_Border]          = ImVec4(0.25f, 0.25f, 0.28f, 0.6f);
    colors[ImGuiCol_FrameBg]         = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]  = ImVec4(0.26f, 0.29f, 0.33f, 1.00f);
    colors[ImGuiCol_FrameBgActive]   = ImVec4(0.26f, 0.29f, 0.33f, 1.00f);
    colors[ImGuiCol_TitleBg]         = ImVec4(0.13f, 0.24f, 0.42f, 1.00f);
    colors[ImGuiCol_TitleBgActive]   = ImVec4(0.18f, 0.34f, 0.60f, 1.00f);
    colors[ImGuiCol_Tab]             = ImVec4(0.16f, 0.28f, 0.48f, 1.00f);
    colors[ImGuiCol_TabActive]       = ImVec4(0.20f, 0.40f, 0.68f, 1.00f);
    colors[ImGuiCol_TabHovered]      = ImVec4(0.25f, 0.45f, 0.78f, 1.00f);
    colors[ImGuiCol_Button]          = ImVec4(0.18f, 0.28f, 0.48f, 1.00f);
    colors[ImGuiCol_ButtonHovered]   = ImVec4(0.25f, 0.35f, 0.58f, 1.00f);
    colors[ImGuiCol_ButtonActive]    = ImVec4(0.30f, 0.40f, 0.70f, 1.00f);

    colors[ImGuiCol_Text]               = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_FrameBg]            = ImVec4(0.20f, 0.21f, 0.25f, 1.00f);

    colors[ImGuiCol_Header]             = ImVec4(0.25f, 0.28f, 0.40f, 1.00f);
    colors[ImGuiCol_HeaderHovered]      = ImVec4(0.30f, 0.35f, 0.55f, 1.00f);
    colors[ImGuiCol_HeaderActive]       = ImVec4(0.35f, 0.40f, 0.60f, 1.00f);
    colors[ImGuiCol_PopupBg]            = ImVec4(0.16f, 0.16f, 0.20f, 1.00f);


    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontDefault();
    ImFont* customFont = io.Fonts->AddFontFromFileTTF("C:\\Users\\arunc\\Desktop\\Engine Project\\Engine\\editor\\Roboto-Regular.ttf", 18.0f);
    if (customFont) {
        io.FontDefault = customFont;
    }
}
#endif //IMGUISTYLECONFIG_HPP
