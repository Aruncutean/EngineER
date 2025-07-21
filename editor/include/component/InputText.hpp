//
// Created by arunc on 02/07/2025.
//

#ifndef INPUTTEXT_HPP
#define INPUTTEXT_HPP

#include <imgui.h>
#include "IComponent.hpp"
#include <string>
#include <functional>

namespace component {
    class InputText : public IComponent {
    public:
        using OnChangeCallback = std::function<void(const std::string &)>;

        InputText(const std::string &label, size_t maxLen = 256)
            : label(label), maxLength(maxLen), buffer(maxLen, '\0') {
        }

        void initialize() override {
        }

        void render() override {
            std::string oldValue(buffer.data());

            if (disabled)
                ImGui::BeginDisabled();

            if (ImGui::InputText(("##" + label).c_str(), buffer.data(), maxLength)) {
                std::string newValue(buffer.data());
                if (newValue != oldValue) {
                    changed = true;
                    if (onChange) onChange(newValue);
                } else {
                    changed = false;
                }
            } else {
                changed = false;
            }


            if (disabled)
                ImGui::EndDisabled();
        }

        bool WasChanged() const {
            return changed;
        }

        std::string GetValue() const {
            return std::string(buffer.data());
        }

        void SetValue(const std::string &val) {
            std::fill(buffer.begin(), buffer.end(), '\0');
            std::copy_n(val.begin(), std::min(val.size(), maxLength - 1), buffer.begin());
        }

        void SetOnChangeCallback(OnChangeCallback callback) {
            onChange = std::move(callback);
        }


        bool disabled = false;

    private:
        std::string label;
        size_t maxLength;
        std::vector<char> buffer;
        bool changed = false;
        OnChangeCallback onChange;
    };
}

#endif //INPUTTEXT_HPP
