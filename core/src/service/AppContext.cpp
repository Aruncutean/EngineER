//
// Created by arunc on 14/07/2025.
//

#include "service/AppContext.hpp"

std::vector<std::function<void(std::shared_ptr<Entity::Entity>)> > AppContext::addEntity;