//
// Created by arunc on 14/07/2025.
//

#ifndef SCENEHIERARCHYVIEW_HPP
#define SCENEHIERARCHYVIEW_HPP
#include "component/List.hpp"
#include "component/View.hpp"
#include "service/AppContext.hpp"
#include "service/EditorService.hpp"

class SceneHierarchyView : public View {
public:
    SceneHierarchyView() : View("SceneHierarchyView") {
        noCollapse = true;


        auto sceneList = std::make_shared<component::List<std::shared_ptr<Entity::Entity> > >("Entities");

        sceneList->SetItems(AppContext::Instance().GetCurrentWorld().GetEntities());
        sceneList->SetOnSelect([this](const std::shared_ptr<Entity::Entity> &entity) {
            Service::EditorService::Instance().setSelectedEntity(entity.get());
        });


        AppContext::Instance().addEntity.push_back([this, sceneList](std::shared_ptr<Entity::Entity> entity) {
            sceneList->AddItem(entity);
        });


        sceneList->SetToString([](const std::shared_ptr<Entity::Entity> &entity) {
            return entity->Name;
        });

        addComponent(sceneList);
    }
};

#endif //SCENEHIERARCHYVIEW_HPP
