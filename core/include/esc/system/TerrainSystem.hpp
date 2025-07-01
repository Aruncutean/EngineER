//
// Created by arunc on 01/07/2025.
//

#ifndef TERRAINSYSTEM_HPP
#define TERRAINSYSTEM_HPP
#include "esc/component/HeightMapComponent.hpp"
#include "esc/component/MeshComponent.hpp"
#include "esc/component/TransformComponent.hpp"



#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <queue>

namespace System {
	class TerrainSystem {
	public:



		bool Load(const std::string& path, float sx, float sy, int baseRes) {

			return true;
		}

		void Update(const glm::vec3& cameraPos) {

		}

		void ProcessPendingLODs(int maxPerFrame) {

		}

		void Render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) {


			//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	};

}
#endif //TERRAINSYSTEM_HPP
