//
// Created by arunc on 01/07/2025.
//

#ifndef MATERIALCOMPONENT_HPP
#define MATERIALCOMPONENT_HPP
#include "ComponentBase.hpp"
#include <iostream>


namespace Component {

    class MaterialComponent : public ComponentBase
    {
    public:
        std::string MaterialID;

        MaterialComponent() = default;

        std::string GetTypeName() const {
            return "MaterialComponent";
        }

        boost::property_tree::ptree ToPtree() const override {
            boost::property_tree::ptree pt;
            pt.put("MaterialID", MaterialID);
            return pt;
        }

        void FromPtree(const boost::property_tree::ptree& pt) override {
            MaterialID = pt.get<std::string>("MaterialID", "");
        }
    };
}
#endif //MATERIALCOMPONENT_HPP
