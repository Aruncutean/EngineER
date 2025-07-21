//
// Created by arunc on 04/07/2025.
//

#ifndef MAINMENUBAR_HPP
#define MAINMENUBAR_HPP


#include <imgui.h>
#include <functional>
#include <vector>
#include <memory>

#include "IComponent.hpp"
#include "Menu.hpp"

namespace component {
    class MainMenuBar : public IComponent {
    public:
        void AddMenu(const std::shared_ptr<Menu> &menu) {
            menus.push_back(menu);
        }

        void initialize() override {
        };

        void render() override {
            if (ImGui::BeginMainMenuBar()) {
                for (const auto &menu: menus) {
                    if (menu) {
                        menu->Render();
                    }
                }
                ImGui::EndMainMenuBar();
            }
        }

    private:
        std::vector<std::shared_ptr<Menu> > menus;
    };
}

#endif //MAINMENUBAR_HPP
