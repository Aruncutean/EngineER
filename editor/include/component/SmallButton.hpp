//
// Created by arunc on 04/07/2025.
//

#ifndef SMALLBUTTON_HPP
#define SMALLBUTTON_HPP
#include "Button.hpp"
#include "IComponent.hpp"

namespace component {
    class SmallButton : public IComponent {
    public:
        Button(const std::string& label)
            : label(label) {}

        void render() override {
            if (ImGui::Button(label.c_str())) {
                if (onClick) {
                    onClick();
                }
            }
        }

        void initialize() override {
        }

        void SetCallback(const std::function<void()>& callback) {
            onClick = callback;
        }

        void SetLabel(const std::string& newLabel) {
            label = newLabel;
        }

    private:
        std::string label;
        std::function<void()> onClick = nullptr;
    };
}


#endif //SMALLBUTTON_HPP
