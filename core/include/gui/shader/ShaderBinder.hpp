//
// Created by arunc on 01/07/2025.
//

#ifndef SHADERBINDER_HPP
#define SHADERBINDER_HPP

#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
#include <iostream>
#include <boost/multi_index/detail/ord_index_impl.hpp>

class ShaderBinder {
public:
    ShaderBinder(GLuint shaderProgram)
        : program(shaderProgram) {
    }

    void setFloat(const std::string& name, float value) {
        glUniform1f(getLocation(name), value);
    }

    void setInt(const std::string& name, int value) {
        glUniform1i(getLocation(name), value);
    }

    void setUInt(const std::string& name, unsigned int value) {
        glUniform1ui(getLocation(name), value);
    }

    void setVec3(const std::string& name, const glm::vec3& vec) {
        glUniform3f(getLocation(name), vec.x, vec.y, vec.z);
    }

    void setVec2(const std::string& name, const glm::vec2& vec) {
        glUniform2f(getLocation(name), vec.x, vec.y);
    }

    void setVec4(const std::string& name, const glm::vec4& vec) {
        glUniform4f(getLocation(name), vec.x, vec.y, vec.z, vec.w);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(getLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

private:
    GLuint program;
    std::unordered_map<std::string, GLint> uniformCache;

    GLint getLocation(const std::string& name) {
        if (uniformCache.find(name) != uniformCache.end()) {
            return uniformCache[name];
        }

        GLint location = glGetUniformLocation(program, name.c_str());
        if (location == -1) {
            std::cerr << "[Warning] Uniform '" << name << "' not found in shader.\n";
        }

        uniformCache[name] = location;
        return location;
    }
};

#endif //SHADERBINDER_HPP
