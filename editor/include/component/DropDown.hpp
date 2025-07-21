//
// Created by arunc on 15/07/2025.
//

#ifndef DROPDOWN_HPP
#define DROPDOWN_HPP
#include "IComponent.hpp"
#include <imgui.h>
#include <string>
#include <vector>
#include <functional>
#include <boost/uuid/uuid_generators.hpp>

namespace component {
    template<typename T>
    class DropDown : public IComponent {
    public:
        using OnChangeCallback = std::function<void(const T &)>;
        using DisplayFunc = std::function<std::string(const T &)>;

        DropDown(const std::string &label,
                 const std::vector<T> &items,
                 int *selectedIndex,
                 DisplayFunc displayFunc)
            : label(label), items(items), selectedIndex(selectedIndex), displayFunc(displayFunc) {
            boost::uuids::random_generator generator;
            id = *reinterpret_cast<int *>(&generator());
        }

        void initialize() override {
        }

        void render() override {
            if (!selectedIndex || items.empty()) return;

            if (disabled) ImGui::BeginDisabled();

            if (ImGui::Combo((label + "##" + std::to_string(id)).c_str(), selectedIndex,
                             [](void *data, int idx, const char **out_text) {
                                 auto &vec = *static_cast<std::vector<T> *>(data);
                                 if (idx < 0 || idx >= static_cast<int>(vec.size())) return false;
                                 *out_text = vec[idx].name.c_str();
                                 return true;
                             }, static_cast<void *>(&items), items.size())) {
                if (onChange && *selectedIndex >= 0 && *selectedIndex < static_cast<int>(items.size()))
                    onChange(items[*selectedIndex]);
            }

            if (disabled) ImGui::EndDisabled();
        }

        void SetOnChangeCallback(OnChangeCallback cb) {
            onChange = std::move(cb);
        }

        void SetItems(const std::vector<T> &newItems) {
            items = newItems;
        }

        const T *GetSelectedItem() const {
            if (!selectedIndex || *selectedIndex < 0 || *selectedIndex >= static_cast<int>(items.size()))
                return nullptr;
            return &items[*selectedIndex];
        }

        bool disabled = false;

    private:
        std::string label;
        std::vector<T> items;
        int *selectedIndex = nullptr;
        DisplayFunc displayFunc;
        OnChangeCallback onChange;
        int id = 0;
    };
}

#endif //DROPDOWN_HPP
