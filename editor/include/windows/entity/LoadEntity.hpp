#ifndef LOADENTITY_HPP
#define LOADENTITY_HPP

#include <fbxsdk.h>
#include "model/MeshData.hpp"
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

class LoadEntity {
public:
    LoadEntity() {
        manager = FbxManager::Create();
        if (!manager) throw std::runtime_error("Failed to create FbxManager");

        ios = FbxIOSettings::Create(manager, IOSROOT);
        manager->SetIOSettings(ios);
    }

    ~LoadEntity() {
        if (manager) manager->Destroy();
    }

    Model::MeshData LoadMesh(const std::string &filePath) {
        FbxImporter *importer = FbxImporter::Create(manager, "");
        if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings())) {
            throw std::runtime_error("Failed to initialize FBX importer");
        }

        FbxScene *scene = FbxScene::Create(manager, "Scene");
        importer->Import(scene);
        importer->Destroy();

        FbxNode *root = scene->GetRootNode();
        if (!root) throw std::runtime_error("No root node in FBX scene");

        Model::MeshData data;
        TraverseNode(root, data);
        return data;
    }

private:
    FbxManager *manager = nullptr;
    FbxIOSettings *ios = nullptr;

    void TraverseNode(FbxNode *node, Model::MeshData &data) {
        if (!node) return;

        FbxMesh *mesh = node->GetMesh();
        if (mesh) ExtractMesh(mesh, data);

        for (int i = 0; i < node->GetChildCount(); ++i) {
            TraverseNode(node->GetChild(i), data);
        }
    }

    void ExtractMesh(FbxMesh *mesh, Model::MeshData &data) {
        mesh->GenerateNormals();
        mesh->GenerateTangentsData();

        FbxStringList uvSetNames;
        mesh->GetUVSetNames(uvSetNames);
        FbxString name = (uvSetNames.GetCount() > 0) ? uvSetNames[0] : FbxString("");
        const char* uvSetName = name.Buffer();

        for (int i = 0; i < mesh->GetPolygonCount(); ++i) {
            int polygonSize = mesh->GetPolygonSize(i);
            for (int j = 0; j < polygonSize; ++j) {
                int ctrlPointIndex = mesh->GetPolygonVertex(i, j);

                // Position
                FbxVector4 pos = mesh->GetControlPointAt(ctrlPointIndex);
                glm::vec3 position = {
                    static_cast<float>(pos[0]) * 0.01f,
                    static_cast<float>(pos[1]) * 0.01f,
                    static_cast<float>(pos[2]) * 0.01f
                };
                data.positions.push_back(position);

                // Normal
                FbxVector4 normal;
                mesh->GetPolygonVertexNormal(i, j, normal);
                glm::vec3 norm = {
                    static_cast<float>(normal[0]),
                    static_cast<float>(normal[1]),
                    static_cast<float>(normal[2])
                };
                data.normals.push_back(norm);

                // UV
                glm::vec2 texCoord = {0.0f, 0.0f};
                if (uvSetName && *uvSetName) {
                    FbxVector2 uv;
                    bool unmapped;
                    if (mesh->GetPolygonVertexUV(i, j, uvSetName, uv, unmapped)) {
                        texCoord = {
                            static_cast<float>(uv[0]),
                            1.0f - static_cast<float>(uv[1]) // flip V
                        };
                    }
                }
                data.uvs.push_back(texCoord);

                // Tangent (optional)
                glm::vec3 tangent = {0.0f, 0.0f, 0.0f};
                if (mesh->GetElementTangentCount() > 0) {
                    FbxGeometryElementTangent *tangentElem = mesh->GetElementTangent();
                    if (tangentElem &&
                        tangentElem->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
                        int tangentIndex = i * polygonSize + j;
                        if (tangentElem->GetReferenceMode() == FbxGeometryElement::eDirect &&
                            tangentIndex < tangentElem->GetDirectArray().GetCount()) {
                            FbxVector4 tan = tangentElem->GetDirectArray().GetAt(tangentIndex);
                            tangent = {
                                static_cast<float>(tan[0]),
                                static_cast<float>(tan[1]),
                                static_cast<float>(tan[2])
                            };
                        }
                    }
                }
                data.tangents.push_back(tangent);


                data.indices.push_back(static_cast<uint32_t>(data.positions.size() - 1));
            }
        }
    }
};

#endif // LOADENTITY_HPP
