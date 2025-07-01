//
// Created by arunc on 01/07/2025.
//

#ifndef MESHCOMPONENT_HPP
#define MESHCOMPONENT_HPP
#include "ComponentBase.hpp"
#include <string>
#include <memory>
#include <iostream>
#include "model/MeshData.hpp"
#include "gui/mesh/GLMesh.hpp"
#include "gui/mesh/MeshRegistry.hpp"
#include "service/AppContext.hpp"

namespace Component {

    class MeshComponent : public ComponentBase
    {
    public:
        std::string meshPath;

        // Not serializable
        std::shared_ptr<Model::MeshData> runtimeMesh;
        std::unique_ptr<GLMesh> glMesh = nullptr;

        MeshComponent() = default;

        void LoadMesh() {

            auto project = AppContext::Instance().GetCurrentProject();

            runtimeMesh = MeshRegistry::GetMesh(project.path + "/" + meshPath + ".mesh.json");
            if (runtimeMesh) {
                glMesh = std::make_unique<GLMesh>(*runtimeMesh);
            }
            else {
                std::cerr << "Failed to load mesh: " << meshPath << std::endl;
            }
        }

        void UploadMesh(const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec3>& normals,
            const std::vector<glm::vec2>& uvs,
            const std::vector<unsigned int>& indices) {

            auto mesh = std::make_shared<Model::MeshData>();
            mesh->positions = positions;
            mesh->normals = normals;
            mesh->uvs = uvs;
            mesh->indices = indices;

            runtimeMesh = mesh;
            glMesh = std::make_unique<GLMesh>(*runtimeMesh);
        }


        std::string GetTypeName() const  {
            return "MeshComponent";
        }

        boost::property_tree::ptree ToPtree() const  {
            boost::property_tree::ptree pt;
            pt.put("MeshPath", meshPath);
            return pt;
        }

        void FromPtree(const boost::property_tree::ptree& pt)  {
            meshPath = pt.get<std::string>("MeshPath", "");
            LoadMesh();
        }



    };


}
#endif //MESHCOMPONENT_HPP
