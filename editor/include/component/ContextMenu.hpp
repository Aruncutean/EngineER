//
// Created by arunc on 03/07/2025.
//

#ifndef CONTEXTMENU_HPP
#define CONTEXTMENU_HPP
#include <imgui.h>
#include <functional>
#include <string>
#include <vector>

template <typename T>
class ContextMenu {
public:
    void AddItem(const std::string& label, std::function<void(const T&)> callback) {
        items.emplace_back(label, std::move(callback));
    }

    void Show(const std::string& id, const T& value) {
        if (ImGui::BeginPopupContextItem(id.c_str())) {
            for (const auto& [label, action] : items) {
                if (ImGui::MenuItem(label.c_str())) {
                    if (action) action(value);
                }
            }
            ImGui::EndPopup();
        }
    }

    void Clear() {
        items.clear();
    }

private:
    std::vector<std::pair<std::string, std::function<void(const T&)>>> items;
};

#endif //CONTEXTMENU_HPP
