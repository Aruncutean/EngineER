//
// Created by arunc on 01/07/2025.
//

#ifndef PHYSICSCOMPONENT_HPP
#define PHYSICSCOMPONENT_HPP

#include "ComponentBase.hpp"
#include <string>
#include <glm.hpp>
#include "bullet/btBulletDynamicsCommon.h"
#include <boost/property_tree/ptree.hpp>

namespace Component {

    class PhysicsComponent : public ComponentBase
    {
    public:
        float mass = 1.0f;
        bool isKinematic = true;
        bool physicsReady = false;

        btRigidBody* rigidBody = nullptr;

        btCollisionShape* shape = nullptr;
        btDefaultMotionState* motionState = nullptr;

        ~PhysicsComponent() {
            delete rigidBody;
            delete motionState;
            delete shape;
        }

        PhysicsComponent() = default;

        std::string GetTypeName() const  {
            return "PhysicsComponent";
        }
        boost::property_tree::ptree ToPtree() const  {
            boost::property_tree::ptree pt;
            pt.put<float>("mass", mass);
            pt.put<bool>("isKinematic", isKinematic);

            return pt;
        }

        void FromPtree(const boost::property_tree::ptree& pt)  {
            mass = pt.get<float>("mass", 1.0f);
            isKinematic = pt.get<bool>("isKinematic", true);
        }
    };
}
#endif //PHYSICSCOMPONENT_HPP
