//
// Created by arunc on 06/07/2025.
//

#ifndef GRID_HPP
#define GRID_HPP

#include <imgui.h>
#include <vector>
#include <memory>
#include <algorithm>

#include "IComponent.hpp"

namespace component {
    class Grid : public IComponent {
    public:
        Grid(float itemWidth, float itemHeight)
            : itemSize(itemWidth, itemHeight) {
        }

        void AddItem(const std::shared_ptr<IComponent> &item) {
            if (item) items.push_back(item);
        }


        void ClearItems() {
            items.clear();
        }

        void initialize() override {
        }

        void render() override {

            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float padding = 0.0f;
            float cellWidth = itemSize.x + spacing;

            float availWidth = ImGui::GetContentRegionAvail().x;
            int columns = std::max(1, static_cast<int>((availWidth + spacing) / cellWidth));

            int columnIndex = 0;
            for (size_t i = 0; i < items.size(); ++i) {
                items[i]->render();

                columnIndex++;
                if (columnIndex < columns) {
                    ImGui::SameLine();
                } else {
                    columnIndex = 0;
                }
            }
        }



    private:
        std::vector<std::shared_ptr<IComponent>> items;
        ImVec2 itemSize;
    };
}
#endif //GRID_HPP
