#version 450 core
layout(quads, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightMap;
uniform float terrainSize;
uniform float heightScale;

out vec3 worldPos;
out vec2 uvCoords;
out vec3 normal; // optional

void main() {
    vec2 uv = mix(gl_in[0].gl_Position.xy, gl_in[3].gl_Position.xy, gl_TessCoord.xy);
    uvCoords = uv / terrainSize;

    float height = texture(heightMap, uvCoords).r * heightScale;

    worldPos = vec3(uv.x, height, uv.y);
    gl_Position = projection * view * model * vec4(worldPos, 1.0);
}
