#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 WorldPos;

out vec4 FragColor;

uniform vec3 camPos;

struct Material {
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;

    bool useAlbedoMap;
    bool useNormalMap;
    bool useMetallicMap;
    bool useRoughnessMap;
    bool useAOMap;

    vec3 albedoColor;
    vec3 emissive;
    float metallic;
    float roughness;
    float aoValue;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
};

#define NR_POINT_LIGHTS 30

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int numPointLights;

in mat3 TBN;

const float PI = 3.14159265359;

vec3 getNormalFromMap() {
    if(!material.useNormalMap) {
        return normalize(Normal);
    }

    vec3 tangentNormal = texture(material.normalMap, TexCoords).rgb;
    tangentNormal = tangentNormal * 2.0 - 1.0;
    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    vec3 albedo = material.useAlbedoMap ? texture(material.albedoMap, TexCoords).rgb : material.albedoColor;

    float metallic = material.useMetallicMap ? texture(material.metallicMap, TexCoords).r : material.metallic;
    float roughness = material.useRoughnessMap ? texture(material.roughnessMap, TexCoords).r : material.roughness;
    float ao = material.useAOMap ? texture(material.aoMap, TexCoords).r : material.aoValue;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 Lo = vec3(0.0);
    
// Direct light (Directional Light)
    vec3 L = normalize(-dirLight.direction);
    vec3 H = normalize(V + L);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
    float NdotL = max(dot(N, L), 0.0);

    vec3 directLight = (kD * albedo / PI + specular) * dirLight.color * dirLight.intensity * NdotL;

    Lo += directLight;

    for(int i = 0; i < numPointLights; i++) {
        vec3 L = normalize(pointLights[i].position - WorldPos);
        vec3 H = normalize(V + L);

        float distance = length(pointLights[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);

        vec3 radiance = pointLights[i].color * pointLights[i].intensity * attenuation;

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
        float NdotL = max(dot(N, L), 0.0);

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
