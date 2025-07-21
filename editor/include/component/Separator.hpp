//
// Created by arunc on 11/07/2025.
//

#ifndef SEPARATOR_HPP
#define SEPARATOR_HPP
#include <imgui.h>

#include "IComponent.hpp"

namespace component {
    class Separator : public IComponent {
    public:
        Separator() {
        }

        void render() override {
            ImGui::Separator();
        }

        void initialize() override {
        }
    };
}

#endif //SEPARATOR_HPP
