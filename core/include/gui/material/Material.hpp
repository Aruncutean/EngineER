//
// Created by arunc on 01/07/2025.
//

#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <string>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include "gui/shader/ShaderTypes.hpp"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

struct ColorOrTexture {
	std::string color = "#ffffff";
	std::string texturePath;
	bool isTexture = false;

	boost::property_tree::ptree toPtree() const {
		boost::property_tree::ptree pt;
		pt.put("Color", color);
		pt.put("TexturePath", texturePath);
		pt.put("IsTexture", isTexture);
		return pt;
	}

	static ColorOrTexture fromPtree(const boost::property_tree::ptree& pt) {
		ColorOrTexture c;
		c.color = pt.get<std::string>("Color", "#ffffff");
		c.texturePath = pt.get<std::string>("TexturePath", "");
		c.isTexture = pt.get<bool>("IsTexture", false);
		return c;
	}
};

struct FloatOrTexture {
	float value = 0;
	std::string texturePath;
	bool isTexture = false;

	boost::property_tree::ptree toPtree() const {
		boost::property_tree::ptree pt;
		pt.put("Value", value);
		pt.put("TexturePath", texturePath);
		pt.put("IsTexture", isTexture);
		return pt;
	}

	static FloatOrTexture fromPtree(const boost::property_tree::ptree& pt) {
		FloatOrTexture c;
		c.value = pt.get<float>("Value", 0);
		c.texturePath = pt.get<std::string>("TexturePath", "");
		c.isTexture = pt.get<bool>("IsTexture", false);
		return c;
	}
};


namespace Material {

	class MaterialBase {
	public:
		std::string id;
		std::string name;
		std::string path;

		virtual ShaderType getShader() const = 0;
		virtual boost::property_tree::ptree toPtree() const = 0;
		~MaterialBase() = default;

		static std::shared_ptr<MaterialBase> fromPtree(const boost::property_tree::ptree& pt);

	};

	class MaterialPhong : public MaterialBase {
	public:
		ColorOrTexture ambient;
		ColorOrTexture diffuse;
		ColorOrTexture specular;
		float shininess = 32.0f;

		MaterialPhong() = default;

		MaterialPhong(const boost::property_tree::ptree& pt) {
			shininess = pt.get<float>("Shininess", 32.0f);
			ambient = ColorOrTexture::fromPtree(pt.get_child("Ambient"));
			diffuse = ColorOrTexture::fromPtree(pt.get_child("Diffuse"));
			specular = ColorOrTexture::fromPtree(pt.get_child("Specular"));
		}

		ShaderType getShader() const override { return ShaderType::Phong; }

		boost::property_tree::ptree toPtree() const override {
			boost::property_tree::ptree pt;
			pt.put("$type", "phong");
			pt.put("Shininess", shininess);
			pt.add_child("Ambient", ambient.toPtree());
			pt.add_child("Diffuse", diffuse.toPtree());
			pt.add_child("Specular", specular.toPtree());
			pt.put("Id", id);
			pt.put("Name", name);
			pt.put("Path", path);
			return pt;
		}
	};

	class MaterialPBR : public MaterialBase {
	public:
		ColorOrTexture albedo;
		std::string normal = "";
		FloatOrTexture metallic;
		FloatOrTexture roughness;
		FloatOrTexture ao;

		MaterialPBR() = default;

		MaterialPBR(const boost::property_tree::ptree& pt) {
			albedo = ColorOrTexture::fromPtree(pt.get_child("Albedo"));
			normal = pt.get<std::string>("Normal", "");
			metallic = FloatOrTexture::fromPtree(pt.get_child("Metallic"));
			roughness = FloatOrTexture::fromPtree(pt.get_child("Roughness"));
			ao = FloatOrTexture::fromPtree(pt.get_child("AO"));
		}

		ShaderType getShader() const override { return ShaderType::PBR; }

		boost::property_tree::ptree toPtree() const override {
			boost::property_tree::ptree pt;
			pt.put("$type", "pbr");

			pt.add_child("Albedo", albedo.toPtree());
			pt.put("Normal", normal);
			pt.add_child("Metallic", metallic.toPtree());
			pt.add_child("Roughness", roughness.toPtree());
			pt.add_child("AO", ao.toPtree());


			pt.put("id", id);
			pt.put("name", name);
			pt.put("path", path);
			return pt;
		}
	};

	class MaterialDefault : public MaterialBase {
	public:
		float diffuseColor[3] = { 1, 1, 1 };
		float specularColor[3] = { 1, 1, 1 };
		float shininess = 32.0f;

		MaterialDefault() = default;

		MaterialDefault(const boost::property_tree::ptree& pt) {
			shininess = pt.get<float>("shininess", 32.0f);
			diffuseColor[0] = pt.get<float>("diffuseColor.x", 1.0f);
			diffuseColor[1] = pt.get<float>("diffuseColor.y", 1.0f);
			diffuseColor[2] = pt.get<float>("diffuseColor.z", 1.0f);
			specularColor[0] = pt.get<float>("specularColor.x", 1.0f);
			specularColor[1] = pt.get<float>("specularColor.y", 1.0f);
			specularColor[2] = pt.get<float>("specularColor.z", 1.0f);

		}

		ShaderType getShader() const override { return ShaderType::Basic; }

		boost::property_tree::ptree toPtree() const override {
			boost::property_tree::ptree pt;
			pt.put("$type", "default");
			pt.put("shininess", shininess);
			pt.put("diffuseColor.x", diffuseColor[0]);
			pt.put("diffuseColor.y", diffuseColor[1]);
			pt.put("diffuseColor.z", diffuseColor[2]);
			pt.put("specularColor.x", specularColor[0]);
			pt.put("specularColor.y", specularColor[1]);
			pt.put("specularColor.z", specularColor[2]);
			pt.put("id", id);
			pt.put("name", name);
			pt.put("path", path);
			return pt;
		}
	};

}

#endif //MATERIAL_HPP
