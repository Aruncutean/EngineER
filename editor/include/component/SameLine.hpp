//
// Created by arunc on 03/07/2025.
//

#ifndef SAMELINE_HPP
#define SAMELINE_HPP
#include <imgui.h>
#include <memory>
#include <vector>

#include "IComponent.hpp"

namespace component {
    class SameLine : public IComponent {
    public:
        void initialize() override {
        }

        void render() override {
            bool first = true;
            for (auto &c: components) {
                if (!first) ImGui::SameLine();
                c->render();
                first = false;
            }
        }

        void AddComponent(const std::shared_ptr<IComponent> &component) {
            components.push_back(component);
        }

        void RemoveComponent(const std::shared_ptr<IComponent> &component) {
            components.erase(std::remove(components.begin(), components.end(), component), components.end());
        }

    private:
        std::vector<std::shared_ptr<IComponent> > components;
    };
}

#endif //SAMELINE_HPP
