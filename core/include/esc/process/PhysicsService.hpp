//
// Created by arunc on 01/07/2025.
//

#ifndef PHYSICSSERVICE_HPP
#define PHYSICSSERVICE_HPP
#include <memory>
#include "esc/component/PhysicsComponent.hpp"

namespace Physics {

    class PhysicsService {

    public:
        PhysicsService() = default;
        ~PhysicsService() = default;


        std::shared_ptr<Component::PhysicsComponent> CreateBoxWithUserData(glm::vec3 pos, glm::vec3 size, float mass, void* userData) {
            auto phys = std::make_shared<Component::PhysicsComponent>();

            btCollisionShape* shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
            btTransform transform;
            transform.setIdentity();
            transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

            btVector3 inertia(0, 0, 0);
            if (mass != 0.0f)
                shape->calculateLocalInertia(mass, inertia);

            btDefaultMotionState* motion = new btDefaultMotionState(transform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motion, shape, inertia);

            phys->shape = shape;
            phys->motionState = motion;
            phys->rigidBody = new btRigidBody(rbInfo);
            phys->rigidBody->setUserPointer(userData);

            return phys;
        }




    };


}
#endif //PHYSICSSERVICE_HPP
