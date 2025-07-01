//
// Created by arunc on 01/07/2025.
//

#ifndef MESHDATA_HPP
#define MESHDATA_HPP
#include <vector>
#include <glm.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

namespace Model {
	struct MeshData {
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		std::vector<unsigned int> indices;

		static MeshData fromPtree(const boost::property_tree::ptree& pt) {
			MeshData meshData;

			// Parse positions
			for (const auto& item : pt.get_child("Positions")) {
				const auto& node = item.second;
				meshData.positions.emplace_back(
					node.get<float>("X", 0.0f),
					node.get<float>("Y", 0.0f),
					node.get<float>("Z", 0.0f)
				);
			}

			// Parse normals
			for (const auto& item : pt.get_child("Normals")) {
				const auto& node = item.second;
				meshData.normals.emplace_back(
					node.get<float>("X", 0.0f),
					node.get<float>("Y", 0.0f),
					node.get<float>("Z", 0.0f)
				);
			}

			for (const auto& item : pt.get_child("UVs")) {
				const auto& node = item.second;
				meshData.uvs.emplace_back(
					node.get<float>("X", 0.0f),
					node.get<float>("Y", 0.0f)
				);
			}

			if (pt.count("Tangents") > 0) {
				for (const auto& item : pt.get_child("Tangents")) {
					const auto& node = item.second;
					meshData.tangents.emplace_back(
						node.get<float>("X", 0.0f),
						node.get<float>("Y", 0.0f),
						node.get<float>("Z", 0.0f)
					);
				}
			}

			// Parse bitangents (optional)
			if (pt.count("Bitangents") > 0) {
				for (const auto& item : pt.get_child("Bitangents")) {
					const auto& node = item.second;
					meshData.bitangents.emplace_back(
						node.get<float>("X", 0.0f),
						node.get<float>("Y", 0.0f),
						node.get<float>("Z", 0.0f)
					);
				}
			}
			// Parse indices (flat array — stored as values)
			for (const auto& item : pt.get_child("Indices")) {
				try {
					meshData.indices.push_back(item.second.get_value<unsigned int>());
				}
				catch (...) {
					// fallback if value is a string: try convert
					meshData.indices.push_back(std::stoul(item.second.get_value<std::string>()));
				}
			}

			return meshData;
		}

	};
}

#endif //MESHDATA_HPP
