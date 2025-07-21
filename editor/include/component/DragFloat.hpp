//
// Created by arunc on 07/07/2025.
//

#ifndef DRAGFLOAT_HPP
#define DRAGFLOAT_HPP

#include <imgui.h>
#include "IComponent.hpp"
#include <string>
#include <functional>

namespace component {
    class DragFloat : public IComponent {
    public:
        using OnChangeCallback = std::function<void(float)>;

        DragFloat(const std::string &label, float *value, float speed = 0.1f, float min = 0.0f, float max = 0.0f)
            : label(label), value(value), speed(speed), min(min), max(max) {}

        void initialize() override {}

        void render() override {
            if (!value) return;

            float old = *value;
            if (disabled)
                ImGui::BeginDisabled();

            if (ImGui::DragFloat(("##" + label).c_str(), value, speed, min, max)) {
                if (onChange && *value != old)
                    onChange(*value);
            }

            if (disabled)
                ImGui::EndDisabled();
        }

        void SetOnChangeCallback(OnChangeCallback callback) {
            onChange = std::move(callback);
        }

        bool disabled = false;

    private:
        std::string label;
        float *value;
        float speed;
        float min;
        float max;
        OnChangeCallback onChange;
    };
}

#endif // DRAGFLOAT_HPP

