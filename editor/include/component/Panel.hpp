//
// Created by arunc on 11/07/2025.
//

#ifndef PANEL_HPP
#define PANEL_HPP
#include <memory>
#include <vector>

#include "IComponent.hpp"

namespace component {
    class Panel : public IComponent {
    public:
        void addComponent(const std::shared_ptr<component::IComponent> &component) {
            components.push_back(component);
        }

        void removeComponent(const std::shared_ptr<component::IComponent> &component) {
            components.erase(std::remove(components.begin(), components.end(), component), components.end());
        }

        void render() override {
            for (auto &component: components) {
                if (component != nullptr)
                    component->render();
            }
        };

        void initialize() override {
        };

        void clear() {
            components.clear();
        }

    private:
        std::vector<std::shared_ptr<component::IComponent> > components;
    };
}

#endif //PANEL_HPP
