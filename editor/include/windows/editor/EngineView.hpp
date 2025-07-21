//
// Created by arunc on 04/07/2025.
//

#ifndef ENGINEVIEW_HPP
#define ENGINEVIEW_HPP


#include "component/Checkbox.hpp"
#include "component/ComboBox.hpp"
#include "component/DragFloat.hpp"
#include "component/DragFloat3.hpp"
#include "component/InputText.hpp"
#include "component/Label.hpp"
#include "component/MainMenuBar.hpp"
#include "component/Panel.hpp"
#include "component/Separator.hpp"

#include "component/View.hpp"
#include "esc/component/LightComponent.hpp"
#include "esc/component/MaterialComponent.hpp"
#include "esc/component/PhysicsComponent.hpp"
#include "esc/component/TransformComponent.hpp"
#include "service/EditorService.hpp"
#include "esc/entity/Entity.hpp"
#include "service/AssetsService.hpp"


class EngineView : public View {
public:
    EngineView(): View("EngineView") {
        noCollapse = true;

        Service::EditorService::Instance().setNewEntityCallback([this](Entity::Entity *entity) {
            setPanel(Service::EditorService::Instance().GetSelectedEntity());
        });

        if (auto selectedEntity = Service::EditorService::Instance().GetSelectedEntity()) {
            setPanel(selectedEntity);
        }
    }


    void setPanel(Entity::Entity *entity) {
        if (entity) {
            clearComponent();
            if (entity->HasComponent<Component::TransformComponent>()) {
                auto panel = std::make_shared<component::Panel>();

                auto transform = entity->GetComponent<Component::TransformComponent>();
                auto physicsComponent = entity->GetComponent<Component::PhysicsComponent>();


                auto inputText = std::make_shared<component::InputText>("EntityName");
                inputText->SetValue(entity->Name);
                inputText->SetOnChangeCallback([entity](const std::string &newName) {
                    entity->Name = newName;
                });

                panel->addComponent(inputText);


                auto positionEdit = std::make_shared<component::DragFloat3>(
                    "PositionEntity", &transform->position, 0.1f);
                positionEdit->SetOnChangeCallback(
                    [physicsComponent, transform](glm::vec3 *newPos) {
                        transform->position = *newPos;
                        if (physicsComponent) {
                            btTransform newTransform;
                            newTransform.setOrigin(btVector3(transform->position.x, transform->position.y,
                                                             transform->position.z));
                            btQuaternion rotQuat;
                            rotQuat.setEulerZYX(glm::radians(transform->rotation.z),
                                                glm::radians(transform->rotation.y),
                                                glm::radians(transform->rotation.x));
                            newTransform.setRotation(rotQuat);

                            physicsComponent->rigidBody->setWorldTransform(newTransform);
                            if (physicsComponent->motionState)
                                physicsComponent->motionState->setWorldTransform(newTransform);

                            if (btCollisionShape *shape = physicsComponent->rigidBody->getCollisionShape()) {
                                shape->setLocalScaling(
                                    btVector3(transform->scale.x, transform->scale.y, transform->scale.z));
                            }
                        }
                    });

                auto eulerRotation = std::make_shared<glm::vec3>(glm::degrees(glm::eulerAngles(transform->rotation)));
                auto rotationEdit = std::make_shared<
                    component::DragFloat3>("RotationEntity", eulerRotation.get(), 0.1f);
                rotationEdit->SetOnChangeCallback(
                    [physicsComponent, transform, eulerRotation](glm::vec3 *newRot) {
                        *eulerRotation = *newRot;
                        transform->rotation = glm::quat(glm::radians(*eulerRotation));
                        transform->rotation = glm::quat(glm::radians(*newRot));
                        if (physicsComponent) {
                            btTransform newTransform;
                            newTransform.setOrigin(btVector3(transform->position.x, transform->position.y,
                                                             transform->position.z));
                            btQuaternion rotQuat;
                            rotQuat.setEulerZYX(glm::radians(transform->rotation.z),
                                                glm::radians(transform->rotation.y),
                                                glm::radians(transform->rotation.x));
                            newTransform.setRotation(rotQuat);

                            physicsComponent->rigidBody->setWorldTransform(newTransform);
                            if (physicsComponent->motionState)
                                physicsComponent->motionState->setWorldTransform(newTransform);

                            if (btCollisionShape *shape = physicsComponent->rigidBody->getCollisionShape()) {
                                shape->setLocalScaling(
                                    btVector3(transform->scale.x, transform->scale.y, transform->scale.z));
                            }
                        }
                    });
                auto scaleEdit = std::make_shared<component::DragFloat3>(
                    "ScaleEntity", &transform->scale, 0.1f, 0.01f, 100.0f);
                scaleEdit->SetOnChangeCallback(
                    [ physicsComponent, transform](glm::vec3 *newScale) {
                        transform->scale = *newScale;
                        if (physicsComponent) {
                            btTransform newTransform;
                            newTransform.setOrigin(btVector3(transform->position.x, transform->position.y,
                                                             transform->position.z));
                            btQuaternion rotQuat;
                            rotQuat.setEulerZYX(glm::radians(transform->rotation.z),
                                                glm::radians(transform->rotation.y),
                                                glm::radians(transform->rotation.x));
                            newTransform.setRotation(rotQuat);

                            physicsComponent->rigidBody->setWorldTransform(newTransform);
                            if (physicsComponent->motionState)
                                physicsComponent->motionState->setWorldTransform(newTransform);

                            if (btCollisionShape *shape = physicsComponent->rigidBody->getCollisionShape()) {
                                shape->setLocalScaling(
                                    btVector3(transform->scale.x, transform->scale.y, transform->scale.z));
                            }
                        }
                    }
                );

                panel->addComponent(positionEdit);
                panel->addComponent(rotationEdit);
                panel->addComponent(scaleEdit);


                addComponent(panel);
            }


            if (entity->HasComponent<Component::MaterialComponent>()) {
                auto materialComp = entity->GetComponent<Component::MaterialComponent>();


                auto panelMaterial = std::make_shared<component::Panel>();
                auto separator = std::make_shared<component::Separator>();
                auto Label = std::make_shared<component::Label>("Material Properties");

                panelMaterial->addComponent(separator);
                panelMaterial->addComponent(Label);


                std::vector<Model::AssetItem> allAssets = Service::AssetsService::Instance().getAssets();
                std::vector<std::string> materialPaths;
                std::vector<Model::AssetItem> materialAssets;

                materialPaths.push_back("Default");
                Model::AssetItem defaultAsset;
                defaultAsset.id = "default";
                defaultAsset.name = "Default";
                materialAssets.push_back(defaultAsset);


                for (const auto &asset: allAssets) {
                    if (asset.type == Model::AssetType::Material) {
                        materialPaths.push_back(asset.name);
                        materialAssets.push_back(asset);
                    }
                }

                int currentIndex = 0;
                for (int i = 0; i < materialAssets.size(); ++i) {
                    if (materialAssets[i].id == materialComp->MaterialID) {
                        currentIndex = i;
                        break;
                    }
                }

             auto shaderCombo = std::make_shared<component::ComboBox>("Material", materialPaths, currentIndex);
                shaderCombo->SetOnChangeCallback([this,materialPaths, materialComp, materialAssets](int index) {
                    if (index >= 0 && index < materialPaths.size()) {
                        materialComp->MaterialID = materialAssets[index].id;
                    }
                });

                panelMaterial->addComponent(shaderCombo);
                addComponent(panelMaterial);
            }

            if (entity->HasComponent<Component::LightComponent>()) {

                auto lightComp = entity->GetComponent<Component::LightComponent>();
                auto panel = std::make_shared<component::Panel>();
                auto separator = std::make_shared<component::Separator>();

                const char *lightTypes[] = {"Point", "Directional", "Spot"};
                int currentType = static_cast<int>(lightComp->Type);

                // if (ImGui::Combo("Light Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
                //     lightComp->Type = static_cast<Light::LightType>(currentType);
                //     switch (lightComp->Type) {
                //         case Light::LightType::Point:
                //             lightComp->LightBase = std::make_shared<Light::LightPoint>();
                //             break;
                //         case Light::LightType::Directional:
                //             lightComp->LightBase = std::make_shared<Light::LightDirectional>();
                //             break;
                //         case Light::LightType::Spot:
                //             lightComp->LightBase = std::make_shared<Light::LightSpot>();
                //             break;
                //     }
                // }
                //
                // if (lightComp->LightBase) {
                //     if (auto point = std::dynamic_pointer_cast<Light::LightPoint>(lightComp->LightBase)) {
                //         ImGui::SliderFloat("Intensity", &point->intensity, 0.0f, 1000.0f);
                //         ImGui::ColorEdit3("Color", glm::value_ptr(point->color));
                //         ImGui::SliderFloat("Range", &point->range, 0.0f, 100.0f);
                //     }
                //
                //     if (auto dir = std::dynamic_pointer_cast<Light::LightDirectional>(lightComp->LightBase)) {
                //         ImGui::SliderFloat("Intensity", &dir->intensity, 0.0f, 1000.0f);
                //         ImGui::ColorEdit3("Color", glm::value_ptr(dir->color));
                //         ImGui::DragFloat3("Direction", glm::value_ptr(dir->direction), 0.1f);
                //     }
                //
                //
                //     if (auto spot = std::dynamic_pointer_cast<Light::LightSpot>(lightComp->LightBase)) {
                //         ImGui::DragFloat3("Direction", glm::value_ptr(spot->direction), 0.1f);
                //         ImGui::SliderFloat("Range", &spot->range, 0.0f, 100.0f);
                //         ImGui::SliderFloat("Cutoff", &spot->cutoff, 0.0f, 90.0f);
                //     }
                // }

            }

            if (entity->HasComponent<Component::PhysicsComponent>()) {
                auto physicsComponent = entity->GetComponent<Component::PhysicsComponent>();
                auto panel = std::make_shared<component::Panel>();
                auto separator = std::make_shared<component::Separator>();
                auto Label = std::make_shared<component::Label>("Physics Properties");

                panel->addComponent(separator);
                panel->addComponent(Label);

                auto massEdit = std::make_shared<component::DragFloat>(
                    "Mass", &physicsComponent->mass, 0.1f, 0.0f, 100.0f);

                auto isKinematicCheckbox = std::make_shared<component::Checkbox>(
                    "Is Kinematic", &physicsComponent->isKinematic);

                panel->addComponent(massEdit);
                panel->addComponent(isKinematicCheckbox);

                addComponent(panel);
            }
        }
    }
};


#endif //ENGINEVIEW_HPP
