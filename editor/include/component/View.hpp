//
// Created by arunc on 02/07/2025.
//

#ifndef VIEW_HPP
#define VIEW_HPP

#include <imgui.h>
#include <memory>
#include <string>

#include "IComponent.hpp"
#include "vector"


class View : public std::enable_shared_from_this<View> {
public:
    View(std::string name): name(std::move(name)) {
    }

    ~View() {
    }

    void render();

    void addComponent(const std::shared_ptr<component::IComponent> &component) {
        components.push_back(component);
    }

    void removeComponent(const std::shared_ptr<component::IComponent> &component) {
        components.erase(std::remove(components.begin(), components.end(), component), components.end());
    }

    void clearComponent() {
        components.clear();
    }

    bool alwaysAutoResize = false;
    bool noTitleBar = false;
    bool noResize = false;
    bool noMove = false;
    bool noScrollbar = false;
    bool noCollapse = true;
    bool noBackground = false;
    bool isOpen = true;
    bool enableDockspace = false;
    bool useViewportLayout = false;
    bool setWindowStyle = true;
    std::string name;

    bool operator==(const View &other) const {
        return this->name == other.name;
    }

private:
    std::vector<std::shared_ptr<component::IComponent> > components;
};

#endif //VIEW_HPP
