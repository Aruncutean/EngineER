#version 450 core

layout(location = 0) in vec2 inPosition; // pozitie 2D pe plan XZ

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0);
}
