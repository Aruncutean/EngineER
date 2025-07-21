//
// Created by arunc on 06/07/2025.
//

#ifndef ASSETITEMC_HPP
#define ASSETITEMC_HPP
#include <imgui.h>

#include "IComponent.hpp"
#include "model/AssetItem.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "service/AssetsService.hpp"

namespace component {
    class AssetItem : public IComponent {
    public:
        AssetItem(const std::string &name, Model::AssetItem &assetItem)
            : name(name), assetItem(assetItem) {
            addIcon();
            boost::uuids::random_generator generator;
            boost::uuids::uuid id = generator();
            this->id = *reinterpret_cast<int *>(&id);
        }

        void render() {
            ImGui::PushID(id);
            ImGui::BeginGroup();
            ImVec2 iconSize(64, 64);
            GLuint icon = 0;
            switch (assetItem.type) {
                case Model::AssetType::Folder:
                    icon = icons[3];
                    break;
                case Model::AssetType::Mesh:
                    icon = icons[0];
                    break;
                case Model::AssetType::Texture:
                    icon = icons[1];
                    break;
                case Model::AssetType::Script:
                    icon = icons[2];
                    break;
                case Model::AssetType::Material:
                    icon = icons[4];
                    break;
                default:
                    break;
            }

            UI::ImageButton((std::string("icon_") + std::to_string(id)).c_str(),
                            (ImTextureID) (intptr_t) icon, iconSize,
                            [&]() {
                                if (onClick) onClick(assetItem);
                            }
            );

            float cursorX = ImGui::GetCursorPosX();
            float textAreaWidth = iconSize.x + 10;

            ImGui::PushTextWrapPos(cursorX + textAreaWidth);

            if (renaming) {
                ImGui::SetNextItemWidth(iconSize.x + 10);
                bool enterPressed = ImGui::InputText("##rename", renameBuffer, sizeof(renameBuffer),
                                                     ImGuiInputTextFlags_EnterReturnsTrue |
                                                     ImGuiInputTextFlags_AutoSelectAll);


                if (enterPressed) {
                    assetItem.name = std::string(renameBuffer);
                    renaming = false;
                    Service::AssetsService::Instance().renameAsset(assetItem.id, assetItem.name);
                }


                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    assetItem.name = std::string(renameBuffer);
                    Service::AssetsService::Instance().renameAsset(assetItem.id, assetItem.name);
                    renaming = false;
                }

                if (ImGui::IsItemDeactivated()) {
                    renaming = false;
                }
            } else {
                ImGui::TextWrapped("%s", assetItem.name.c_str());

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                    renaming = true;
                    std::strncpy(renameBuffer, assetItem.name.c_str(), sizeof(renameBuffer));
                }
            }

            ImGui::PopTextWrapPos();
            ImGui::EndGroup();
            ImGui::PopID();
        };

        void initialize() {
        };


        void addIcon() {
            icons[0] = UI::LoadTextureFromFile("assets/geometry.png");
            icons[1] = UI::LoadTextureFromFile("assets/image.png");
            icons[2] = UI::LoadTextureFromFile("assets/script.png");
            icons[3] = UI::LoadTextureFromFile("assets/folder.png");
            icons[4] = UI::LoadTextureFromFile("assets/material.png");
        }


        void SetCallback(const std::function<void(Model::AssetItem)> &callback) {
            onClick = callback;
        }

    private:
        int id = 0;
        GLuint icons[5];
        std::string name;
        std::string path;
        Model::AssetItem assetItem;

        bool renaming = false;
        char renameBuffer[256] = {};
        std::function<void(Model::AssetItem)> onClick = nullptr;
    };
}

#endif //ASSETITEMC_HPP
