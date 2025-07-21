#ifndef SLIDERFLOAT_HPP
#define SLIDERFLOAT_HPP

#include "IComponent.hpp"
#include <imgui.h>
#include <string>
#include <functional>

namespace component {

    class SliderFloat : public IComponent {
    public:
        using OnChangeCallback = std::function<void(float)>;

        SliderFloat(const std::string& label,
                    float* value,
                    float min = 0.0f,
                    float max = 1.0f,
                    const std::string& format = "%.3f")
            : label(label), value(value), min(min), max(max), format(format) {}

        void initialize() override {}

        void render() override {
            if (!value) return;

            float old = *value;

            if (disabled) ImGui::BeginDisabled();

            if (ImGui::SliderFloat(("##" + label).c_str(), value, min, max, format.c_str())) {
                if (onChange && *value != old)
                    onChange(*value);
            }

            if (disabled) ImGui::EndDisabled();

            // Optional label
            ImGui::SameLine();
            ImGui::Text("%s", label.c_str());
        }

        void SetOnChangeCallback(OnChangeCallback cb) {
            onChange = std::move(cb);
        }

        float GetValue() const {
            return value ? *value : 0.0f;
        }

        void SetValue(float val) {
            if (value) *value = val;
        }

        bool disabled = false;

    private:
        std::string label;
        float* value;
        float min;
        float max;
        std::string format;
        OnChangeCallback onChange;
    };

}

#endif // SLIDERFLOAT_HPP
