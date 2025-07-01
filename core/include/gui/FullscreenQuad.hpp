//
// Created by arunc on 01/07/2025.
//

#ifndef FULLSCREENQUAD_HPP
#define FULLSCREENQUAD_HPP
#include <glad/glad.h>
#include <memory>

class FullscreenQuad {
public:
    static std::shared_ptr<FullscreenQuad> Create() {
        auto quad = std::make_shared<FullscreenQuad>();
        quad->init();
        return quad;
    }

    void Render() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    ~FullscreenQuad() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

private:
    GLuint VAO = 0, VBO = 0;

    void init() {
        float quadVertices[] = {
            // positions   // texcoords (optional)
            -1.0f,  1.0f,
            -1.0f, -1.0f,
             1.0f, -1.0f,

            -1.0f,  1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    }
};

#endif //FULLSCREENQUAD_HPP
