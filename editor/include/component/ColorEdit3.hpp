//
// Created by arunc on 11/07/2025.
//

#ifndef COLOREDIT3_HPP
#define COLOREDIT3_HPP

#include "IComponent.hpp"
#include <imgui.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <functional>

namespace component {
    class ColorEdit3 : public IComponent {
    public:
        using OnChangeCallback = std::function<void(const std::string)>;

        ColorEdit3(const std::string &label, std::string hex)
            : label(label), color(hex) {
            boost::uuids::random_generator generator;
            boost::uuids::uuid id = generator();
            this->id = *reinterpret_cast<int *>(&id);
        }

        void initialize() override {
        }

        void render() override {


            std::string old = color;

            if (disabled)
                ImGui::BeginDisabled();


            float color[3] = { 1.0f, 1.0f, 1.0f };

            if (this->color.size() == 7 && this->color[0] == '#') {
                int r, g, b;
                sscanf_s(this->color.c_str(), "#%02x%02x%02x", &r, &g, &b);
                color[0] = r / 255.0f;
                color[1] = g / 255.0f;
                color[2] = b / 255.0f;
            }
            if (ImGui::ColorEdit3(("##" + (label + std::to_string(id))).c_str(), color)) {
                if (onChange ) {
                    char buffer[8];
                    snprintf(buffer, sizeof(buffer), "#%02x%02x%02x",
                        (int)(color[0] * 255),
                        (int)(color[1] * 255),
                        (int)(color[2] * 255));
                    this->color = buffer;
                    onChange(this->color);

                }

            }

            if (disabled)
                ImGui::EndDisabled();

            ImGui::SameLine();
            ImGui::Text("%s", label.c_str());
        }

        void SetOnChangeCallback(OnChangeCallback cb) {
            onChange = std::move(cb);
        }

        std::string GetValue() const {
            return color ;
        }

        void SetValue(std::string &val) {
            color = val;
        }

        bool disabled = false;

    private:
        int id = 0;
        std::string label;
        std::string color = "#fff";
        OnChangeCallback onChange;
    };
}

#endif // COLOREDIT3_HPP
