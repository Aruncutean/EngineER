//
// Created by arunc on 04/07/2025.
//

#ifndef BREADCRUMB_HPP
#define BREADCRUMB_HPP
#include "IComponent.hpp"

namespace component {
    class Breadcrumb : public IComponent {
    public:
        void render() override {
            if (currentPath.empty()) return;

            std::filesystem::path pathAccum;
            size_t id = 0;
            size_t start = 0;

            while (start < currentPath.size()) {
                size_t end = currentPath.find('/', start);
                if (end == std::string::npos) end = currentPath.size();

                std::string part = currentPath.substr(start, end - start);
                if (!part.empty()) {
                    pathAccum /= part;

                    if (id > 0) ImGui::SameLine();

                    std::string label = part + "##" + std::to_string(id);
                    if (ImGui::SmallButton(label.c_str())) {
                        if(currentPath != pathAccum.string()) {
                            currentPath = pathAccum.string();
                            onClick(pathAccum.generic_string());
                        }

                    }

                    if (end < currentPath.size()) {
                        ImGui::SameLine();
                        ImGui::TextUnformatted(">");
                        ImGui::SameLine();
                    }

                    ++id;
                }

                start = end + 1;
            }
        }

        void SetCallback(const std::function<void(const std::string &)> &callback) {
            onClick = callback;
        }

        void initialize() override {
        }

        std::string getCurrentPath() {
            return currentPath;
        }

        void setCurrentPath(const std::string &path) {
            currentPath = path;
        }

    private:
        std::string currentPath = "Assets";
        std::function<void(const std::string &)> onClick = nullptr;
    };
}

#endif //BREADCRUMB_HPP
