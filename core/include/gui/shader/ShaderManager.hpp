//
// Created by arunc on 01/07/2025.
//

#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP
#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderTypes.hpp"

class ShaderManager {
public:
	static void Init() {
		shaders.clear();
	}

	static void LoadShaders() {
		Load(ShaderType::Basic, "shader/basic.vert", "shader/basic.frag");
		// Load(ShaderType::Gizmo, "shader/gizmo.vert", "shader/gizmo.frag");
		Load(ShaderType::PBR, "shader/pbr.vert", "shader/pbr.frag");
		Load(ShaderType::Phong, "shader/phong.vert", "shader/phong.frag");
		Load(ShaderType::SkyAtmosphere, "shader/SkyAtmosphere/sky.vert", "shader/SkyAtmosphere/sky.frag");
		Load(ShaderType::TerrainPBR, "shader/terrain/terrain.vert", "shader/terrain/terrain.frag", "", "shader/terrain/terrain.tesc", "shader/terrain/terrain.tese");
	}

	static GLuint Get(ShaderType id) {
		if (shaders.find(id) == shaders.end())
			throw std::runtime_error("Shader not loaded.");

		return shaders.at(id);
	}

private:
	static std::unordered_map<ShaderType, GLuint> shaders;

	static GLuint Load(ShaderType id, const std::string& vertPath, const std::string& fragPath) {
		return Load(id, vertPath, fragPath, "", "", "");
	}

	static GLuint Load(ShaderType id,
		const std::string& vertPath,
		const std::string& fragPath,
		const std::string& geomPath,
		const std::string& tescPath,
		const std::string& tesePath) {
		if (shaders.find(id) != shaders.end())
			return shaders[id];

		std::string vertexCode = readFile(vertPath);
		std::string fragmentCode = readFile(fragPath);

		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

		GLuint geometryShader = 0;
		if (!geomPath.empty()) {
			std::string geometryCode = readFile(geomPath);
			geometryShader = compileShader(GL_GEOMETRY_SHADER, geometryCode);
		}

		GLuint tessControlShader = 0;
		if (!tescPath.empty()) {
			std::string tescCode = readFile(tescPath);
			tessControlShader = compileShader(GL_TESS_CONTROL_SHADER, tescCode);
		}

		GLuint tessEvalShader = 0;
		if (!tesePath.empty()) {
			std::string teseCode = readFile(tesePath);
			tessEvalShader = compileShader(GL_TESS_EVALUATION_SHADER, teseCode);
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		if (geometryShader) glAttachShader(program, geometryShader);
		if (tessControlShader) glAttachShader(program, tessControlShader);
		if (tessEvalShader) glAttachShader(program, tessEvalShader);

		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[1024];
			glGetProgramInfoLog(program, 1024, nullptr, infoLog);
			throw std::runtime_error("Shader linking failed: " + std::string(infoLog));
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryShader) glDeleteShader(geometryShader);
		if (tessControlShader) glDeleteShader(tessControlShader);
		if (tessEvalShader) glDeleteShader(tessEvalShader);

		shaders[id] = program;
		return program;
	}

	static GLuint compileShader(GLenum type, const std::string& source) {
		GLuint shader = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (!status) {
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			throw std::runtime_error("Shader compilation failed: " + std::string(infoLog));
		}

		return shader;
	}

	static std::string readFile(const std::string& path) {
		std::ifstream file(path);
		if (!file)
			throw std::runtime_error("Failed to open shader file: " + path);

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
};

#endif //SHADERMANAGER_HPP
