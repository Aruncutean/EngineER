#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    bool useAmbientMap;
    bool useDiffuseMap;
    bool useSpecularMap;
    sampler2D ambientMap;
    sampler2D diffuseMap;
    sampler2D specularMap;
    float shininess;
}; 

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};


#define NR_POINT_LIGHTS 30

uniform DirectionalLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;
uniform Material material;
uniform int numPointLights;

vec3 CalcDirLight(vec3 normal, vec3 viewDir, DirectionalLight light, vec3 amb, vec3 diff, vec3 spec);


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 lightDir,
                    vec3 amb, vec3 diff, vec3 spec);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Material input
    vec3 baseAmbient  = material.useAmbientMap  ? texture(material.ambientMap, TexCoords).rgb  : material.ambientColor;
    vec3 baseDiffuse  = material.useDiffuseMap  ? texture(material.diffuseMap, TexCoords).rgb  : material.diffuseColor;
    vec3 baseSpecular = material.useSpecularMap ? texture(material.specularMap, TexCoords).rgb : material.specularColor;

    vec3 result = vec3(0.0);

    // Direcțională
    result += CalcDirLight(norm, viewDir, dirLight, baseAmbient, baseDiffuse, baseSpecular);

    // Punctiforme
    for (int i = 0; i < numPointLights; ++i)
    {
        vec3 lightDir = normalize(pointLights[i].position - FragPos);
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, lightDir, baseAmbient, baseDiffuse, baseSpecular);
   }

    FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(vec3 normal, vec3 viewDir, DirectionalLight light, vec3 amb, vec3 diff, vec3 spec)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambientResult = 0.1 * light.color * amb;
    float d = max(dot(normal, lightDir), 0.0);
    vec3 diffuseResult = d * light.color * diff;
    vec3 reflectDir = reflect(-lightDir, normal);
    float s = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularResult = s * light.color * spec;

    return (ambientResult + diffuseResult + specularResult) * light.intensity;
}



vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 lightDir,
                    vec3 amb, vec3 diff, vec3 spec)
{
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambientResult = 0.1 * light.color * amb;
    float d = max(dot(normal, lightDir), 0.0);
    vec3 diffuseResult = d * light.color * diff;
    vec3 reflectDir = reflect(-lightDir, normal);
    float s = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularResult = s * light.color * spec;

    return (ambientResult + diffuseResult + specularResult) * attenuation * light.intensity;
}
