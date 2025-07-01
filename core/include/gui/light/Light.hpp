//
// Created by arunc on 01/07/2025.
//

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm.hpp>
#include <memory>
#include <boost/property_tree/ptree.hpp>

namespace Light {

	enum class LightType {
		Point,
		Directional,
		Spot
	};

	struct LightBase {
		virtual ~LightBase() = default;
		virtual LightType GetType() const = 0;
		virtual boost::property_tree::ptree ToPtree() const = 0;
	};

	struct LightPoint : public LightBase {
		float intensity = 1.0f;
		float range = 10.0f;
		glm::vec3 color = glm::vec3(1.0f);

		LightType GetType() const override { return LightType::Point; }

		boost::property_tree::ptree ToPtree() const {
			boost::property_tree::ptree pt;
			pt.put("$type", "LightPoint");
			pt.put("intensity", intensity);
			pt.put("range", range);
			pt.put("color.r", color.r);
			pt.put("color.g", color.g);
			pt.put("color.b", color.b);
			return pt;
		}
	};

	struct LightDirectional : public LightBase {
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
		float intensity = 1.0f;
		glm::vec3 color = glm::vec3(1.0f);

		LightType GetType() const override { return LightType::Directional; }

		boost::property_tree::ptree ToPtree() const {
			boost::property_tree::ptree pt;
			pt.put("$type", "LightDirectional");
			pt.put("intensity", intensity);
			pt.put("direction.x", direction.x);
			pt.put("direction.y", direction.y);
			pt.put("direction.z", direction.z);
			pt.put("color.r", color.r);
			pt.put("color.g", color.g);
			pt.put("color.b", color.b);
			return pt;
		}

	};

	struct LightSpot : public LightBase {
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
		float intensity = 1.0f;
		float range = 10.0f;
		float cutoff = 30.0f;
		glm::vec3 color = glm::vec3(1.0f);

		LightType GetType() const override { return LightType::Spot; }

		boost::property_tree::ptree ToPtree() const {
			boost::property_tree::ptree pt;
			pt.put("$type", "LightSpot");
			pt.put("intensity", intensity);
			pt.put("range", range);
			pt.put("cutoff", cutoff);
			pt.put("direction.x", direction.x);
			pt.put("direction.y", direction.y);
			pt.put("direction.z", direction.z);
			pt.put("color.r", color.r);
			pt.put("color.g", color.g);
			pt.put("color.b", color.b);
			return pt;
		}

	};

	inline std::shared_ptr<Light::LightBase> LightFromPtree(const boost::property_tree::ptree& pt) {
		std::string type = pt.get<std::string>("$type");

		if (type == "LightPoint") {
			auto light = std::make_shared<Light::LightPoint>();
			light->intensity = pt.get<float>("intensity", 1.0f);
			light->range = pt.get<float>("range", 10.0f);
			light->color = glm::vec3(
				pt.get<float>("color.r", 1.0f),
				pt.get<float>("color.g", 1.0f),
				pt.get<float>("color.b", 1.0f)
			);
			return light;
		}
		else if (type == "LightDirectional") {
			auto light = std::make_shared<Light::LightDirectional>();
			light->intensity = pt.get<float>("intensity", 1.0f);
			light->direction = glm::vec3(
				pt.get<float>("direction.x", 0.0f),
				pt.get<float>("direction.y", -1.0f),
				pt.get<float>("direction.z", 0.0f)
			);
			light->color = glm::vec3(
				pt.get<float>("color.r", 1.0f),
				pt.get<float>("color.g", 1.0f),
				pt.get<float>("color.b", 1.0f)
			);
			return light;
		}
		else if (type == "LightSpot") {
			auto light = std::make_shared<Light::LightSpot>();
			light->intensity = pt.get<float>("intensity", 1.0f);
			light->range = pt.get<float>("range", 10.0f);
			light->cutoff = pt.get<float>("cutoff", 30.0f);
			light->direction = glm::vec3(
				pt.get<float>("direction.x", 0.0f),
				pt.get<float>("direction.y", -1.0f),
				pt.get<float>("direction.z", 0.0f)
			);
			light->color = glm::vec3(
				pt.get<float>("color.r", 1.0f),
				pt.get<float>("color.g", 1.0f),
				pt.get<float>("color.b", 1.0f)
			);
			return light;
		}

		throw std::runtime_error("Unknown light type: " + type);
	}




} // namespace Graphics::Light

#endif //LIGHT_HPP
