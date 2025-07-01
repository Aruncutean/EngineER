#version 330 core

layout(location = 0) in vec2 aPos;
out vec2 TexCoords;
out vec3 vPosition;
void main()
{
    TexCoords = aPos * 0.5 + 0.5; // convertește NDC [-1,1] la UV [0,1]
    gl_Position = vec4(aPos, 0.0, 1.0);
    vPosition = vec3(aPos,-1);
}
