//
// Created by arunc on 01/07/2025.
//

#ifndef MATERIALIO_HPP
#define MATERIALIO_HPP

#include <string>
#include <vector>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "gui/material/Material.hpp"

class MaterialIO {
public:
    MaterialIO() = default;

    void Save(const std::string& path, const std::shared_ptr<Material::MaterialBase>& material) {
        boost::property_tree::ptree pt = material->toPtree();
        boost::property_tree::write_json(path, pt);
    }

    std::shared_ptr<Material::MaterialBase> Load(const std::string& path) {
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(path, pt);

        std::string type = pt.get<std::string>("$type", "default");

        if (type == "default") {
            return std::static_pointer_cast<Material::MaterialBase>(std::make_shared<Material::MaterialDefault>(pt));
        }
        else if (type == "phong") {
            return std::static_pointer_cast<Material::MaterialBase>(std::make_shared<Material::MaterialPhong>(pt));
        }
        else if (type == "pbr") {
            return std::static_pointer_cast<Material::MaterialBase>(std::make_shared<Material::MaterialPBR>(pt));
        }

        throw std::runtime_error("Unknown material type: " + type);
    }

};

#endif //MATERIALIO_HPP
