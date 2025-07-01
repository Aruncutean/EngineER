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

    Model::MeshData LoadMesh(const std::string& filePath) {
        FbxImporter* importer = FbxImporter::Create(manager, "");
        if (!importer->Initialize(filePath.c_str(), -1, manager->GetIOSettings())) {
            throw std::runtime_error("Failed to initialize FBX importer");
        }

        FbxScene* scene = FbxScene::Create(manager, "Scene");
        importer->Import(scene);
        importer->Destroy();

        FbxNode* root = scene->GetRootNode();
        if (!root) throw std::runtime_error("No root node in FBX scene");

        Model::MeshData data;
        TraverseNode(root, data);
        return data;
    }

private:
    FbxManager* manager = nullptr;
    FbxIOSettings* ios = nullptr;

    void TraverseNode(FbxNode* node, Model::MeshData& data) {
        if (!node) return;

        FbxMesh* mesh = node->GetMesh();
        if (mesh) ExtractMesh(mesh, data);

        for (int i = 0; i < node->GetChildCount(); ++i) {
            TraverseNode(node->GetChild(i), data);
        }
    }

    void ExtractMesh(FbxMesh* mesh, Model::MeshData& data) {
        mesh->GenerateNormals();
        mesh->GenerateTangentsData();

        const int controlPointsCount = mesh->GetControlPointsCount();
        const FbxVector4* controlPoints = mesh->GetControlPoints();

        for (int i = 0; i < controlPointsCount; ++i) {
            FbxVector4 pos = controlPoints[i];
            data.positions.emplace_back(static_cast<float>(pos[0]) * 0.01f,
                                        static_cast<float>(pos[1]) * 0.01f,
                                        static_cast<float>(pos[2]) * 0.01f);
        }

        for (int i = 0; i < mesh->GetPolygonCount(); ++i) {
            for (int j = 0; j < mesh->GetPolygonSize(i); ++j) {
                int ctrlPointIndex = mesh->GetPolygonVertex(i, j);
                data.indices.push_back(ctrlPointIndex);

                // Normals
                FbxVector4 normal;
                mesh->GetPolygonVertexNormal(i, j, normal);
                data.normals.emplace_back((float)normal[0], (float)normal[1], (float)normal[2]);

                // UVs
                FbxVector2 uv;
                bool unmapped;
                if (mesh->GetPolygonVertexUV(i, j, "UVMap", uv, unmapped)) {
                    data.uvs.emplace_back((float)uv[0], (float)uv[1]);
                }

                // Tangents
                if (mesh->GetElementTangentCount() > 0) {
                    FbxGeometryElementTangent* tangentElem = mesh->GetElementTangent();
                    if (tangentElem && tangentElem->GetDirectArray().GetCount() > ctrlPointIndex) {
                        FbxVector4 tan = tangentElem->GetDirectArray().GetAt(ctrlPointIndex);
                        data.tangents.emplace_back((float)tan[0], (float)tan[1], (float)tan[2]);
                    }
                }
            }
        }
    }
};

#endif // LOADENTITY_HPP
