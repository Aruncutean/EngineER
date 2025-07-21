#ifndef ASSETSVIEW_HPP
#define ASSETSVIEW_HPP

#include "MaterialView.hpp"
#include "component/AssetItem.hpp"
#include "component/Breadcrumb.hpp"
#include "component/ContextWindowMenu.hpp"
#include "component/Grid.hpp"
#include "component/View.hpp"
#include "service/AssetsService.hpp"
#include "model/AssetItem.hpp"
#include "service/EditorService.hpp"
#include "service/EntityService.hpp"
#include "windows/entity/LoadEntity.hpp"


class AssetsView : public View {
public:
    AssetsView() : View("AssetsView") {
        noCollapse = true;
        alwaysAutoResize = true;

        grid = std::make_shared<component::Grid>(64, 64);

        breadcrumb = std::make_shared<component::Breadcrumb>();
        breadcrumb->setCurrentPath(currentFolder);
        breadcrumb->SetCallback([this](const std::string &newPath) {
            if (currentFolder != newPath) {
                currentFolder = newPath;
                refreshGrid();
            }
            breadcrumb->setCurrentPath(currentFolder);
        });
        addComponent(breadcrumb);

        auto meshMenu = std::make_shared<component::Menu>("Mesh");
        meshMenu->AddItem("Add", [this]() {
            std::string meshPath = UI::openDialog();
            if (!meshPath.empty()) {
                Model::MeshData meshData = LoadEntity::LoadEntity().LoadMesh(meshPath);
                Service::AssetsService::Instance().SaveMesh(meshData, meshPath, currentFolder);
                refreshGrid();
            }
        });

        auto materialMenu = std::make_shared<component::Menu>("Material");
        materialMenu->AddItem("Add", [this]() {
            Service::AssetsService::Instance().addMaterial(currentFolder);
            refreshGrid();
        });

        auto textureMenu = std::make_shared<component::Menu>("Texture");
        textureMenu->AddItem("Add", [this]() {
            std::string texturePath = UI::openDialog();
            if (!texturePath.empty()) {
                Service::AssetsService::Instance().addTexture(texturePath, currentFolder);
                refreshGrid();
            }
        });

        auto folderMenu = std::make_shared<component::Menu>("Folder");
        folderMenu->AddItem("Add", [this]() {
            Service::AssetsService::Instance().addFolder("New Folder", currentFolder);
            refreshGrid();
        });

        auto heightMapMenu = std::make_shared<component::Menu>("HeightMap");
        heightMapMenu->AddItem("Add", [this]() {
            std::string texturePath = UI::openDialog();
            if (!texturePath.empty()) {
                Service::AssetsService::Instance().addHightMap(texturePath, currentFolder);
                refreshGrid();
            }
        });

        refreshGrid();

        auto contextWindowMenu = std::make_shared<component::ContextWindowMenu>();
        contextWindowMenu->AddMenu(meshMenu);
        contextWindowMenu->AddMenu(materialMenu);
        contextWindowMenu->AddMenu(folderMenu);
        contextWindowMenu->AddMenu(textureMenu);

        addComponent(grid);
        addComponent(contextWindowMenu);
    }

private:
    std::string currentFolder = "Assets";
    std::vector<Model::AssetItem> assetItems;
    std::shared_ptr<component::Grid> grid;
    std::shared_ptr<component::Breadcrumb> breadcrumb;

    void refreshGrid() {
        getCurrentItemsForFolder();
        if (grid) {
            grid->ClearItems();
            for (Model::AssetItem &asset: assetItems) {
                auto item = std::make_shared<component::AssetItem>(asset.name, asset);
                item->SetCallback([this](Model::AssetItem item) {
                        switch (item.type) {
                            case Model::AssetType::Folder: {
                                currentFolder = currentFolder + "/" + item.name;
                                breadcrumb->setCurrentPath(currentFolder);
                                refreshGrid();
                                break;
                            }
                            case Model::AssetType::Mesh: {
                                Service::EntityService enitityService;

                                Entity::Entity *entity = enitityService.create(item.path);

                                if (Service::EditorService::Instance().GetSelectedEntity() == nullptr) {
                                    Service::EditorService::Instance().setSelectedEntity(entity);
                                }

                                AppContext::Instance().AddNewEntity(std::shared_ptr<Entity::Entity>(entity));
                                break;
                            }
                            case Model::AssetType::Material: {
                                Service::EditorService::Instance().setMaterialSelected(&item);
                                if (!service_editor::ViewService::Instance().GetViewByName("MaterialView")) {
                                    service_editor::ViewService::Instance().AddView(std::make_shared<MaterialView>());
                                }
                                break;
                            }
                            case Model::AssetType::Texture: {
                                break;
                            }
                            case Model::AssetType::Script: {
                                // Handle script selection
                                break;
                            }
                            case Model::AssetType::HightMap: {
                                AppContext::Instance().GetCurrentWorld().SetTerrainPath(item.path);
                                break;
                            }
                            default:
                                break;
                        }
                    }
                );
                grid->AddItem(item);
            }
        }
    }

    void getCurrentItemsForFolder() {
        auto &allAssets = Service::AssetsService::Instance().getAssets();
        assetItems.clear();
        assetItems.reserve(allAssets.size());

        for (const auto &asset: allAssets) {
            if (asset.baseDirectory == currentFolder) {
                assetItems.push_back(asset);
            }
        }

        std::sort(assetItems.begin(), assetItems.end(), [](const auto &a, const auto &b) {
            if (a.type == Model::AssetType::Folder && b.type != Model::AssetType::Folder) return true;
            if (a.type != Model::AssetType::Folder && b.type == Model::AssetType::Folder) return false;
            return a.name < b.name;
        });
    }
};

#endif // ASSETSVIEW_HPP
