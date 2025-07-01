//
// Created by arunc on 01/07/2025.
//

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP
#include <glad/glad.h>

class ShaderProgram {
public:
    explicit ShaderProgram(GLuint programId)
        : m_programId(programId) {
    }

    void Use() const {
        glUseProgram(m_programId);
    }

    GLuint GetProgramId() const {
        return m_programId;
    }

private:
    GLuint m_programId;
};

#endif //SHADERPROGRAM_HPP
