//
// Created by arunc on 01/07/2025.
//

#ifndef MESHREGISTRY_HPP
#define MESHREGISTRY_HPP
#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "model/MeshData.hpp"
#include <iostream>

class MeshRegistry {
public:
    static std::shared_ptr<Model::MeshData> GetMesh(const std::string& meshPath) {
        auto it = _loadedMeshes.find(meshPath);
        if (it != _loadedMeshes.end()) {
            return it->second;
        }

        boost::property_tree::ptree pt;
        try {
            boost::property_tree::read_json(meshPath, pt);
        }
        catch (const std::exception& e) {
            std::cerr << "Eroare la citirea mesh-ului: " << e.what() << std::endl;
            return nullptr;
        }

        auto mesh = std::make_shared<Model::MeshData>();
        *mesh = Model::MeshData::fromPtree(pt);

        _loadedMeshes[meshPath] = mesh;
        return mesh;
    }

private:
    static inline std::unordered_map<std::string, std::shared_ptr<Model::MeshData>> _loadedMeshes;
};


#endif //MESHREGISTRY_HPP
