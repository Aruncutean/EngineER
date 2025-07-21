#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "IComponent.hpp"
#include <imgui.h>
#include <string>
#include <functional>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace component {

    class Checkbox : public IComponent {
    public:
        using OnChangeCallback = std::function<void(bool)>;

        Checkbox(const std::string& label, bool* value)
            : label(label), value(value) {
            boost::uuids::random_generator generator;
            id = boost::uuids::to_string(generator());
        }

        void initialize() override {}

        void render() override {
            if (!value) return;

            bool old = *value;

            if (disabled) ImGui::BeginDisabled();

            std::string fullLabel = label + "##" + id;
            if (ImGui::Checkbox(fullLabel.c_str(), value)) {
                if (onChange && *value != old)
                    onChange(*value);
            }

            if (disabled) ImGui::EndDisabled();
        }

        void SetOnChangeCallback(OnChangeCallback cb) {
            onChange = std::move(cb);
        }

        bool GetValue() const {
            return value ? *value : false;
        }

        void SetValue(bool val) {
            if (value) *value = val;
        }

        bool disabled = false;

    private:
        std::string label;
        std::string id;
        bool* value;
        OnChangeCallback onChange;
    };

}

#endif // CHECKBOX_HPP
