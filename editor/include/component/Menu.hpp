//
// Created by arunc on 04/07/2025.
//

#ifndef MENU_HPP
#define MENU_HPP

#include <imgui.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace component {

    class MenuItem {
    public:
        MenuItem(const std::string& label, std::function<void()> callback)
            : label(label), callback(callback) {}

        void Render() const {
            if (ImGui::MenuItem(label.c_str())) {
                if (callback) callback();
            }
        }

    private:
        std::string label;
        std::function<void()> callback;
    };

    class Menu {
    public:
        Menu(const std::string& name) : name(name) {}

        void AddItem(const std::string& label, std::function<void()> callback) {
            items.emplace_back(label, callback);
        }

        void Render() const {
            if (ImGui::BeginMenu(name.c_str())) {
                for (const auto& item : items) {
                    item.Render();
                }
                ImGui::EndMenu();
            }
        }

    private:
        std::string name;
        std::vector<MenuItem> items;
    };

} // namespace component

#endif // MENU_HPP
