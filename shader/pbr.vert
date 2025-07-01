#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

uniform mat4 model, view, projection;
out vec3 WorldPos;
out vec2 TexCoords;
out vec3 Normal;

out mat3 TBN;


void main() {
    WorldPos = vec3(model * vec4(aPos, 1.0));


    vec3 N = normalize(mat3(model) * aNormal);
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(cross(N, T));
    TBN = mat3(T, B, N);

    TexCoords = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(WorldPos, 1.0);
}