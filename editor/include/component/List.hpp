#ifndef LIST_HPP
#define LIST_HPP

#include <imgui.h>
#include <vector>
#include <functional>
#include <string>

#include "ContextMenu.hpp"
#include "IComponent.hpp"

namespace component {
    template<typename T>
    class List : public IComponent {
    public:
        List(const std::string& label) : label(label) {}

        void initialize() override {}

        void render() override {
            ImGui::BeginChild(label.c_str(), ImVec2(0, 150), true);

            for (size_t i = 0; i < items.size(); ++i) {
                bool selected = (selectedIndex == static_cast<int>(i));
                std::string display = toString ? toString(items[i]) : "Item " + std::to_string(i);

                if (ImGui::Selectable(display.c_str(), selected)) {
                    selectedIndex = static_cast<int>(i);
                    if (onSelect) {
                        onSelect(items[i]);
                    }
                }

                contextMenu.Show(display, items[i]);
            }

            ImGui::EndChild();
        }

        void SetItems(const std::vector<T>& newItems) {
            items = newItems;
            selectedIndex = -1;
        }



        void AddItem(const T& item) {
            items.push_back(item);
        }

        void SetOnSelect(std::function<void(const T&)> callback) {
            onSelect = std::move(callback);
        }

        void SetToString(std::function<std::string(const T&)> func) {
            toString = std::move(func);
        }

        ContextMenu<T>& GetContextMenu() {
            return contextMenu;
        }


    private:
        ContextMenu<T> contextMenu;
        std::vector<T> items;
        int selectedIndex = -1;
        std::string label;
        std::function<void(const T&)> onSelect;
        std::function<std::string(const T&)> toString;
    };
}


#endif //INPUT_HPP