//
// Created by arunc on 04/07/2025.
//

#ifndef CONTEXTWINDOWMENU_HPP
#define CONTEXTWINDOWMENU_HPP
#include <imgui.h>
#include <vector>
#include <functional>
#include <string>
#include "IComponent.hpp"
#include "Menu.hpp"

namespace component {
    class ContextWindowMenu : public IComponent {
    public:
        ContextWindowMenu(const std::string& popupId = "window_context")
            : popupId(popupId) {}

        void AddMenu(const std::shared_ptr<Menu> &menu) {
            menus.push_back(menu);
        }

        void SetPopupFlags(ImGuiPopupFlags flags) {
            popupFlags = flags;
        }

        void initialize() override {}

        void render() override {
            if (ImGui::BeginPopupContextWindow(popupId.c_str(), popupFlags)) {
                for (const auto &menu: menus) {
                    if (menu) {
                        menu->Render();
                    }
                }
                ImGui::EndPopup();
            }
        }

    private:
        std::string popupId;
        ImGuiPopupFlags popupFlags = ImGuiPopupFlags_MouseButtonRight;
        std::vector<std::shared_ptr<Menu> > menus;
    };

}
#endif //CONTEXTWINDOWMENU_HPP
