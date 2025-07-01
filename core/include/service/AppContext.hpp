//
// Created by arunc on 01/07/2025.
//

#ifndef APPCONTEXT_HPP
#define APPCONTEXT_HPP
#include "model/ProjectData.hpp"
#include "model/SceneData.hpp"
#include <optional>
#include <memory>
#include <string>
#include <iostream>
#include "model/World.hpp"

class AppContext {
public:
    static AppContext& Instance() {
        static AppContext instance;
        return instance;
    }


    void SetCurrentProject(const Model::ProjectData& project) {
        currentProject = project;
    }

    bool HasCurrentProject() const {
        return currentProject.has_value();
    }

    const Model::ProjectData& GetCurrentProject() const {
        if (!currentProject)
            throw std::runtime_error("No project loaded.");
        return *currentProject;
    }

    void ClearProject() {
        currentProject.reset();
    }

    const Model::SceneData& GetCurrentScene() const {
        if (!curentScene)
            throw std::runtime_error("No project loaded.");
        return *curentScene;
    }

    void SetCurrentScene(const Model::SceneData& sceneData) {
        curentScene = sceneData;
    }

    bool HasCurrentScene() const {
        return curentScene.has_value();
    }

    const Model::SceneData& GetCurrentScenePath() const {
        if (!curentScene)
            throw std::runtime_error("No scene loaded.");
        return *curentScene;
    }

    void ClearScene() {
        curentScene.reset();
    }

    void SetCurrentWorld(const World& sceneData) {
        currentWorld = sceneData;
    }

    World& GetCurrentWorld() {
        if (!currentWorld)
            throw std::runtime_error("No scene loaded.");
        return *currentWorld;
    }

    void AddNewEntity(std::shared_ptr<Entity::Entity> enity) {

        currentWorld->AddEntity(enity);
        addEntity(enity);
    }

    std::function<void(std::shared_ptr<Entity::Entity>) > addEntity;

private:
    AppContext() = default;
    ~AppContext() = default;

    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    std::optional<Model::ProjectData> currentProject;
    std::optional<Model::SceneData> curentScene;
    std::optional<World> currentWorld;
};

#endif //APPCONTEXT_HPP
