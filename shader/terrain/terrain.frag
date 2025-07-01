#version 450 core

in vec3 worldPos;
in vec2 uvCoords;
out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform vec3 lightDir;
uniform vec3 lightColor;

void main() {
    vec3 diffuseColor = texture(diffuseTexture, uvCoords).rgb;

    // Simplu lighting lambertian
    vec3 N = vec3(0.0, 1.0, 0.0);  // simplificat (normala up)
    vec3 L = normalize(-lightDir);
    float diff = max(dot(N, L), 0.2);
    vec3 finalColor = diffuseColor * diff * lightColor;

    FragColor = vec4(finalColor, 1.0);
}
