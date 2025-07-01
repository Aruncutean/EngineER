//
// Created by arunc on 01/07/2025.
//

#ifndef GLMESH_HPP
#define GLMESH_HPP

#include <glad/glad.h>
#include <vector>
#include <cstdint>
#include "model/MeshData.hpp"

class GLMesh {
public:
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	int IndexCount = 0;

	GLMesh(const Model::MeshData& data) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		std::vector<float> vertexData;
		for (size_t i = 0; i < data.positions.size(); ++i) {
			// Position
			vertexData.push_back(data.positions[i].x);
			vertexData.push_back(data.positions[i].y);
			vertexData.push_back(data.positions[i].z);

			// UV
			if (i < data.uvs.size()) {
				vertexData.push_back(data.uvs[i].x);
				vertexData.push_back(data.uvs[i].y);
			}
			else {
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
			}

			// Normal
			vertexData.push_back(data.normals[i].x);
			vertexData.push_back(data.normals[i].y);
			vertexData.push_back(data.normals[i].z);

			// Tangent (nou adaugat)
			if (i < data.tangents.size()) {
				vertexData.push_back(data.tangents[i].x);
				vertexData.push_back(data.tangents[i].y);
				vertexData.push_back(data.tangents[i].z);
			}
			else {
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
				vertexData.push_back(0.0f);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);

		// layout(location = 0) position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// layout(location = 1) UV
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// layout(location = 2) normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// layout(location = 3) tangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);

		IndexCount = static_cast<int>(data.indices.size());
	}
	void Render() {
		if (VAO != 0) {
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr);
		}
	}

	void Dispose() {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}

	~GLMesh() {
		Dispose();
	}
};

#endif //GLMESH_HPP
