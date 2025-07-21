//
// Created by arunc on 07/07/2025.
//

#ifndef DRAGFLOAT3_HPP
#define DRAGFLOAT3_HPP


#include <imgui.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "IComponent.hpp"
#include <string>
#include <functional>
#include <iostream>

namespace component {
    class DragFloat3 : public IComponent {
    public:
        using OnChangeCallback = std::function<void(glm::vec3 *)>;

        DragFloat3(const std::string &label, glm::vec3 *value, float speed = 0.1f, float min = 0.0f, float max = 0.0f)
            : label(label), value(value), speed(speed), min(min), max(max) {
            tmp = *value;
        }

        void initialize() override {
        }

        void render() override {
            if (disabled)
                ImGui::BeginDisabled();

            ImGui::Text("%s", label.c_str());
            ImGui::SameLine();

            if (*value != tmp) {
                tmp = *value;
            }

            if (ImGui::DragFloat3(("##" + label).c_str(), glm::value_ptr(tmp), speed)) {
                *value = tmp;
                if (onChange) onChange(value);
            }


            if (disabled)
                ImGui::EndDisabled();
        }

        void SetOnChangeCallback(OnChangeCallback callback) {
            onChange = std::move(callback);
        }

        glm::vec3 *GetValue() const {
            return value;
        }

        void SetValue(glm::vec3 *val) {
            value = val;
        }

        bool disabled = false;

    private:
        std::string label;
        glm::vec3 *value;
        glm::vec3 tmp ;
        float speed;
        float min;
        float max;
        OnChangeCallback onChange;
    };
}


#endif // DRAGFLOAT3_HPP
