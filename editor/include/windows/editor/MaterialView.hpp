//
// Created by arunc on 09/07/2025.
//

#ifndef MATERIALVIEW_HPP
#define MATERIALVIEW_HPP
#include "component/Checkbox.hpp"
#include "component/ColorEdit3.hpp"
#include "component/ComboBox.hpp"
#include "component/Label.hpp"
#include "component/Panel.hpp"
#include "component/Separator.hpp"
#include "component/SliderFloat.hpp"
#include "component/View.hpp"
#include "gui/material/Material.hpp"
#include "gui/material/MaterialManager.hpp"
#include "service/AssetsService.hpp"
#include "service/EditorService.hpp"

class MaterialView : public View {
public:
    MaterialView(): View("MaterialView") {
        noCollapse = true;

        Service::EditorService::Instance().addMaterialCallback([this](Model::AssetItem *item) {
            clearComponent();
            initialize();
        });

        initialize();
    }

    void initialize() {
        if (Service::EditorService::Instance().getMaterialSelected()) {
            path = Service::EditorService::Instance().getMaterialSelected()->path;
            material = Material::MaterialManager::Get(Service::EditorService::Instance().getMaterialSelected()->id);
        }

        if (!material)
            return;

        static std::vector<std::string> shaderTypes = {"default", "phong", "pbr"};
        int currentShader = 0;

        if (std::dynamic_pointer_cast<Material::MaterialPhong>(material)) {
            currentShader = 1;
        } else if (std::dynamic_pointer_cast<Material::MaterialPBR>(material)) {
            currentShader = 2;
        }

        auto shaderCombo = std::make_shared<component::ComboBox>("Shader Type", shaderTypes, currentShader);
        shaderCombo->SetOnChangeCallback([this](int index) {
            std::shared_ptr<Material::MaterialBase> newMaterial;

            if (index == 0) {
                auto def = std::make_shared<Material::MaterialDefault>();
                def->name = material->name;
                newMaterial = def;
            } else if (index == 1) {
                auto phong = std::make_shared<Material::MaterialPhong>();
                phong->name = material->name;
                newMaterial = phong;
            } else if (index == 2) {
                auto pbr = std::make_shared<Material::MaterialPBR>();
                pbr->name = material->name;
                newMaterial = pbr;
            }

            if (newMaterial) {
                newMaterial->id = material->id;
                material = newMaterial;
                Material::MaterialManager::Register(material->id, material);
            }
            switchShader();
        });


        panelMaterial = std::make_shared<component::Panel>();

        auto buttonSave = std::make_shared<component::Button>("Save");
        buttonSave->SetCallback([this]() {
            save();
        });

        addComponent(std::make_shared<component::Label>("Path: " + path));
        addComponent(std::make_shared<component::Separator>());
        addComponent(std::make_shared<component::Label>(" "));
        addComponent(shaderCombo);
        switchShader();
        addComponent(panelMaterial);
        addComponent(buttonSave);
    }

    void save() {
        MaterialIO().Save(
            AppContext::Instance().GetCurrentProject().path + "/" + Service::EditorService::Instance().
            getMaterialSelected()->path, material);
    }


    std::shared_ptr<component::ComboBox> GetTexture(const char *label, std::string &path) {
        std::vector<Model::AssetItem> textures;
        std::vector<std::string> textureNames;
        std::string defaultTexture = "";
        static int selectedIndex = -1;
        for (const auto &asset: Service::AssetsService::Instance().getAssets()) {
            if (asset.type == Model::AssetType::Texture) {
                textureNames.push_back(asset.name);
                textures.push_back(asset);
                if (asset.id == path) {
                    defaultTexture = asset.name;
                    selectedIndex = static_cast<int>(textureNames.size()) - 1;
                }
            }
        }

        auto shaderCombo = std::make_shared<component::ComboBox>("Texture", textureNames, selectedIndex);
        shaderCombo->SetOnChangeCallback([this, textures, &path](int index) {
            if (index >= 0 && index < textures.size()) {
                path = textures[index].id;
            }
        });

        return shaderCombo;
    }

    void switchShader() {
        if (auto phong = std::dynamic_pointer_cast<Material::MaterialPhong>(material)) {
            RenderPhong(phong);
            material = phong;
        } else if (auto pbr = std::dynamic_pointer_cast<Material::MaterialPBR>(material)) {
            RenderPBR(pbr);
            material = pbr;
        } else if (auto def = std::dynamic_pointer_cast<Material::MaterialDefault>(material)) {
            RenderDefault(def);
            material = def;
        }
    }


    void RenderPBR(const std::shared_ptr<Material::MaterialPBR> &mat) {
        auto matPanel = std::make_shared<component::Panel>();
        RenderColorOrTexture("Albedo", mat->albedo, matPanel);

        auto labelNormal = std::make_shared<component::Label>("Normal");

        matPanel->addComponent(labelNormal);
        auto normalTexture = GetTexture("Normal", mat->normal);
        matPanel->addComponent(normalTexture);

        auto labelMetallic = std::make_shared<component::Label>("Metallic");
        matPanel->addComponent(labelMetallic);

        auto panelMetallic = std::make_shared<component::Panel>();
        auto checkBox = std::make_shared<component::Checkbox>("Is Texture ", &mat->metallic.isTexture);

        checkBox->SetOnChangeCallback([this, mat, panelMetallic](bool isTexture) {
            mat->metallic.isTexture = isTexture;
            panelMetallic->clear();
            if (mat->metallic.isTexture) {
                panelMetallic->addComponent(
                    GetTexture("Metallic", mat->metallic.texturePath));
            } else {
                auto slider = std::make_shared<component::SliderFloat>("Metallic", &mat->metallic.value, 0.0f, 1.0f,
                                                                       "%.02f");
                panelMetallic->addComponent(slider);
            }
        });

        panelMetallic->clear();
        if (mat->metallic.isTexture) {
            panelMetallic->addComponent(
                GetTexture("Metallic", mat->metallic.texturePath));
        } else {
            auto slider = std::make_shared<component::SliderFloat>("Metallic", &mat->metallic.value, 0.0f, 1.0f,
                                                                   "%.02f");
            panelMetallic->addComponent(slider);
        }

        matPanel->addComponent(checkBox);
        matPanel->addComponent(panelMetallic);


        auto labelRoughness = std::make_shared<component::Label>("Roughness");
        matPanel->addComponent(labelRoughness);

        auto panelRoughness = std::make_shared<component::Panel>();
        auto checkBoxRoughness = std::make_shared<component::Checkbox>("Is Texture ", &mat->roughness.isTexture);

        checkBoxRoughness->SetOnChangeCallback([this, mat, panelRoughness](bool isTexture) {
            panelRoughness->clear();
            mat->roughness.isTexture = isTexture;
            if (mat->roughness.isTexture) {
                panelRoughness->addComponent(
                    GetTexture("Roughness", mat->roughness.texturePath));
            } else {
                auto slider = std::make_shared<component::SliderFloat>("Roughness", &mat->roughness.value, 0.0f, 1.0f,
                                                                       "%.02f");
                panelRoughness->addComponent(slider);
            }
        });

        panelRoughness->clear();
        if (mat->roughness.isTexture) {
            panelRoughness->addComponent(
                GetTexture("Roughness", mat->roughness.texturePath));
        } else {
            auto slider = std::make_shared<component::SliderFloat>("Roughness", &mat->roughness.value, 0.0f, 1.0f,
                                                                   "%.02f");
            panelRoughness->addComponent(slider);
        }

        matPanel->addComponent(checkBoxRoughness);
        matPanel->addComponent(panelRoughness);

        auto labelAO = std::make_shared<component::Label>("AO");
        matPanel->addComponent(labelAO);

        auto panelAO = std::make_shared<component::Panel>();
        auto checkBoxAO = std::make_shared<component::Checkbox>("Is Texture ", &mat->ao.isTexture);

        checkBoxAO->SetOnChangeCallback([this, mat, panelAO](bool isTexture) {
            panelAO->clear();
            mat->ao.isTexture = isTexture;
            if (mat->ao.isTexture) {
                panelAO->addComponent(
                    GetTexture("AO", mat->ao.texturePath));
            } else {
                auto slider = std::make_shared<component::SliderFloat>("AO", &mat->ao.value, 0.0f, 1.0f,
                                                                       "%.02f");
                panelAO->addComponent(slider);
            }
        });

        panelAO->clear();
        if (mat->ao.isTexture) {
            panelAO->addComponent(
                GetTexture("AO", mat->ao.texturePath));
        } else {
            auto slider = std::make_shared<component::SliderFloat>("AO", &mat->ao.value, 0.0f, 1.0f,
                                                                   "%.02f");
            panelAO->addComponent(slider);
        }

        matPanel->addComponent(checkBoxAO);
        matPanel->addComponent(panelAO);

        panelMaterial->clear();
        panelMaterial->addComponent(matPanel);
    }

    void RenderColorOrTexture(const char *label, ColorOrTexture &cot, std::shared_ptr<component::Panel> parentPanal) {
        auto matPanel = std::make_shared<component::Panel>();
        matPanel->addComponent(std::make_shared<component::Label>(label));
        matPanel->addComponent(std::make_shared<component::Label>("Is Texture"));


        auto panel = std::make_shared<component::Panel>();
        auto checkbox = std::make_shared<component::Checkbox>("Is Texture", &cot.isTexture);
        checkbox->SetOnChangeCallback([this, &cot,panel](bool isTexture) {
            cot.isTexture = isTexture;
            panel->clear();
            if (cot.isTexture) {
                panel->addComponent(GetTexture("Texture", cot.texturePath));
            } else {
                auto colorEdit = std::make_shared<
                    component::ColorEdit3>("Color", cot.color);
                colorEdit->SetOnChangeCallback([this, &cot](const std::string &color) {
                    cot.color = color;
                });
                panel->addComponent(colorEdit);
            }
        });

        if (cot.isTexture) {
            panel->addComponent(GetTexture("Texture", cot.texturePath));
        } else {
            auto colorEdit = std::make_shared<
                component::ColorEdit3>("Color", cot.color);
            colorEdit->SetOnChangeCallback([this, &cot](const std::string &color) {
                cot.color = color;
            });
            panel->addComponent(colorEdit);
        }

        matPanel->addComponent(checkbox);
        matPanel->addComponent(panel);
        parentPanal->addComponent(matPanel);
    }

    void RenderPhong(const std::shared_ptr<Material::MaterialPhong> &mat) {
        auto matPanel = std::make_shared<component::Panel>();
        matPanel->addComponent(std::make_shared<component::Label>("Shininess"));
        matPanel->addComponent(std::make_shared<component::SliderFloat>("Shininess", &mat->shininess, 0.0f, 256.0f));
        RenderColorOrTexture("Ambient", mat->ambient, matPanel);
        RenderColorOrTexture("Diffuse", mat->diffuse, matPanel);
        RenderColorOrTexture("Specular", mat->specular, matPanel);

        panelMaterial->clear();
        panelMaterial->addComponent(matPanel);
    }


    void RenderDefault(const std::shared_ptr<Material::MaterialDefault> &mat) {
        auto matPanel = std::make_shared<component::Panel>();
        matPanel->addComponent(std::make_shared<component::Label>("Shininess"));
        matPanel->addComponent(std::make_shared<component::SliderFloat>("Shininess", &mat->shininess, 0.0f, 256.0f));
        matPanel->addComponent(std::make_shared<component::Label>("Diffuse Color"));

        char buffer[8];
        snprintf(buffer, sizeof(buffer), "#%02x%02x%02x",
                 (int) (mat->diffuseColor[0] * 255),
                 (int) (mat->diffuseColor[1] * 255),
                 (int) (mat->diffuseColor[2] * 255));

        matPanel->addComponent(
            std::make_shared<component::ColorEdit3>("Diffuse Color", buffer));


        matPanel->addComponent(std::make_shared<component::Label>("Specular Color"));
        char bufferS[8];
        snprintf(bufferS, sizeof(bufferS), "#%02x%02x%02x",
                 (int) (mat->specularColor[0] * 255),
                 (int) (mat->specularColor[1] * 255),
                 (int) (mat->specularColor[2] * 255));
        matPanel->addComponent(
            std::make_shared<component::ColorEdit3>("Specular Color",
                                                    bufferS));

        panelMaterial->clear();
        panelMaterial->addComponent(matPanel);
    }

private:
    std::string path;
    std::shared_ptr<Material::MaterialBase> material;
    std::shared_ptr<component::Panel> panelMaterial;
};

#endif //MATERIALVIEW_HPP
