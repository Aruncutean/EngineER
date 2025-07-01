//
// Created by arunc on 01/07/2025.
//

#ifndef MESHDATASERIALIZER_HPP
#define MESHDATASERIALIZER_HPP
#include "model/MeshData.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Service {

	class MeshDataSerializer {
	public:
		static boost::property_tree::ptree Vec3ToPtree(const glm::vec3& vec) {
			boost::property_tree::ptree pt;
			pt.put<float>("X", static_cast<float>(vec.x));
			pt.put<float>("Y", static_cast<float>(vec.y));
			pt.put<float>("Z", static_cast<float>(vec.z));
			return pt;
		}

		static boost::property_tree::ptree Vec2ToPtree(const glm::vec2& vec) {
			boost::property_tree::ptree pt;
			pt.put<float>("X", static_cast<float>(vec.x));
			pt.put<float>("Y", static_cast<float>(vec.y));
			return pt;
		}

		static glm::vec3 PtreeToVec3(const boost::property_tree::ptree& pt) {
			return glm::vec3(
				pt.get<float>("X"),
				pt.get<float>("Y"),
				pt.get<float>("Z")
			);
		}

		static glm::vec2 PtreeToVec2(const boost::property_tree::ptree& pt) {
			return glm::vec2(
				pt.get<float>("X"),
				pt.get<float>("Y")
			);
		}

		static boost::property_tree::ptree Serialize(const Model::MeshData& mesh) {
			boost::property_tree::ptree root;

			boost::property_tree::ptree posArray;
			for (const auto& v : mesh.positions)
				posArray.push_back(std::make_pair("", Vec3ToPtree(v)));
			root.add_child("Positions", posArray);

			boost::property_tree::ptree normalArray;
			for (const auto& n : mesh.normals)
				normalArray.push_back(std::make_pair("", Vec3ToPtree(n)));
			root.add_child("Normals", normalArray);

			boost::property_tree::ptree uvArray;
			for (const auto& uv : mesh.uvs)
				uvArray.push_back(std::make_pair("", Vec2ToPtree(uv)));
			root.add_child("UVs", uvArray);

			boost::property_tree::ptree tangentArray;
			for (const auto& t : mesh.tangents)
				tangentArray.push_back(std::make_pair("", Vec3ToPtree(t)));
			root.add_child("Tangents", tangentArray);

			boost::property_tree::ptree bitangentsArray;
			for (const auto& bt : mesh.bitangents)
				bitangentsArray.push_back(std::make_pair("", Vec2ToPtree(bt)));
			root.add_child("Bitangents", bitangentsArray);

			boost::property_tree::ptree indexArray;
			for (const auto& i : mesh.indices) {
				boost::property_tree::ptree val;
				val.put<int>("", i);
				indexArray.push_back(std::make_pair("", val));
			}
			root.add_child("Indices", indexArray);

			return root;
		}

		static Model::MeshData Deserialize(const boost::property_tree::ptree& root) {
			Model::MeshData mesh;

			for (const auto& v : root.get_child("Positions"))
				mesh.positions.push_back(PtreeToVec3(v.second));

			for (const auto& n : root.get_child("Normals"))
				mesh.normals.push_back(PtreeToVec3(n.second));

			for (const auto& uv : root.get_child("UVs"))
				mesh.uvs.push_back(PtreeToVec2(uv.second));

			for (const auto& i : root.get_child("Indices"))
				mesh.indices.push_back(i.second.get_value<unsigned int>());

			return mesh;
		}
	};


}
#endif //MESHDATASERIALIZER_HPP
