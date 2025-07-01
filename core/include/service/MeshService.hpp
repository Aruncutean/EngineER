//
// Created by arunc on 01/07/2025.
//

#ifndef MESHSERVICE_HPP
#define MESHSERVICE_HPP
#include "io/MeshDataSerializer.hpp"
#include <fstream>

namespace Service {

    class MeshService
    {
    public:

        static void Save(const Model::MeshData& mesh, const std::string& path) {
            boost::property_tree::ptree root = Service::MeshDataSerializer::Serialize(mesh);
            boost::property_tree::write_json(path, root);
        }

        static Model::MeshData Load(const std::string& path) {
            boost::property_tree::ptree root;
            boost::property_tree::read_json(path, root);
            return Service::MeshDataSerializer::Deserialize(root);
        }

    private:

    };



}


#endif //MESHSERVICE_HPP
