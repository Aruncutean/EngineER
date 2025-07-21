//
// Created by arunc on 02/07/2025.
//

#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP
#include <string>

namespace component {
    class IComponent {
    public:
        virtual ~IComponent() = default;

        virtual void render() = 0;

        virtual void initialize() = 0;

    };
}

#endif //ICOMPONENT_HPP
