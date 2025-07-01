//
// Created by arunc on 01/07/2025.
//

#ifndef TRANSFORMCOMPONENT_HPP
#define TRANSFORMCOMPONENT_HPP

#include "ComponentBase.hpp"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
#include <boost/property_tree/ptree.hpp>

namespace Component {

	class TransformComponent : public ComponentBase
	{
	public:

		glm::vec3 position = glm::vec3(0.0f);
		glm::quat rotation = glm::quat(glm::vec3(0.0f));
		glm::vec3 scale = glm::vec3(1.0f);

		TransformComponent() = default;

		TransformComponent(const boost::property_tree::ptree& pt) {
			position = glm::vec3(
				pt.get<float>("Position.X", 0),
				pt.get<float>("Position.Y", 0),
				pt.get<float>("Position.Z", 0)
			);
			rotation = glm::vec3(
				pt.get<float>("Rotation.X", 0),
				pt.get<float>("Rotation.Y", 0),
				pt.get<float>("Rotation.Z", 0)
			);
			scale = glm::vec3(
				pt.get<float>("Scale.X", 1),
				pt.get<float>("Scale.Y", 1),
				pt.get<float>("Scale.Z", 1)
			);
		}

		glm::mat4 GetTransform() const {
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, position);

			trans *= glm::toMat4(rotation);
			trans = glm::scale(trans, scale);
			return trans;
		}

		boost::property_tree::ptree ToPtree() const  {
			boost::property_tree::ptree pt;
			pt.put("Position.X", position.x);
			pt.put("Position.Y", position.y);
			pt.put("Position.Z", position.z);

			glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
			pt.put("Rotation.X", euler.x);
			pt.put("Rotation.Y", euler.y);
			pt.put("Rotation.Z", euler.z);

			pt.put("Scale.X", scale.x);
			pt.put("Scale.Y", scale.y);
			pt.put("Scale.Z", scale.z);

			return pt;
		}

		void FromPtree(const boost::property_tree::ptree& pt)  {
			position.x = pt.get<float>("Position.X", 0);
			position.y = pt.get<float>("Position.Y", 0);
			position.z = pt.get<float>("Position.Z", 0);

			glm::vec3 eulerAngles = glm::radians(glm::vec3(
				pt.get<float>("Rotation.X", 0),
				pt.get<float>("Rotation.Y", 0),
				pt.get<float>("Rotation.Z", 0)
			));
			rotation = glm::quat(eulerAngles);

			scale.x = pt.get<float>("Scale.X", 1);
			scale.y = pt.get<float>("Scale.Y", 1);
			scale.z = pt.get<float>("Scale.Z", 1);
		}

		std::string GetTypeName()  const  {
			return "TransformComponent";
		}
	};

}
#endif //TRANSFORMCOMPONENT_HPP
