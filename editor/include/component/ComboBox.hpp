#ifndef COMBOBOX_HPP
#define COMBOBOX_HPP

#include "IComponent.hpp"
#include <imgui.h>
#include <string>
#include <vector>
#include <functional>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace component {
    class ComboBox : public IComponent {
    public:
        using OnChangeCallback = std::function<void(int)>;

        ComboBox(const std::string &label,
                 std::vector<std::string> options,
                 int selectedIndex)
            : label(label), options(std::move(options)), selectedIndex(selectedIndex) {
            boost::uuids::random_generator generator;
            boost::uuids::uuid id = generator();
            this->id = *reinterpret_cast<int *>(&id);
        }

        void initialize() override {
        }

        void render() override {
            if (options.empty()) return;

            if (disabled) ImGui::BeginDisabled();

            if (ImGui::Combo((label + std::to_string(id)).c_str(), &selectedIndex,
                             [](void *data, int idx, const char **out_text) {
                                 const auto &opts = *static_cast<std::vector<std::string> *>(data);
                                 if (idx >= 0 && idx < opts.size()) {
                                     *out_text = opts[idx].c_str();
                                     return true;
                                 }
                                 return false;
                             }, static_cast<void *>(&options), static_cast<int>(options.size()))) {
                if (onChange)
                    onChange(selectedIndex);
            }

            if (disabled) ImGui::EndDisabled();
        }

        void SetOptions(const std::vector<std::string> &newOptions) {
            options = newOptions;
        }

        void SetOnChangeCallback(OnChangeCallback cb) {
            onChange = std::move(cb);
        }

        void SetSelectedIndex(int index) {
            if (selectedIndex)
                selectedIndex = index;
        }

        int GetSelectedIndex() const {
            return selectedIndex ? selectedIndex : -1;
        }

        bool disabled = false;

    private:
        int id = 0;
        std::string label;
        std::vector<std::string> options;
        int selectedIndex = 0;
        OnChangeCallback onChange;
    };
}

#endif // COMBOBOX_HPP
