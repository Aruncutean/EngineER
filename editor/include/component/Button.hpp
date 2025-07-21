//
// Created by arunc on 02/07/2025.
//

#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "IComponent.hpp"

#include <imgui.h>
#include <string>
#include <functional>
#include "IComponent.hpp"
namespace component {
     class Button : public IComponent {
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

#endif //BUTTON_HPP
