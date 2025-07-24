#version 420 core
layout(location = 0) in vec3 WorldPos;
layout(locaiton = 1) in vec3 Normal;

layout(location = 0) out vec4 FragColor;

layout (std140, binding = 1) uniform RenderData
{
    vec3 lightPos;
    vec3 lightColor;
    vec3 viewPos;
};
uniform vec3 objectColor;
uniform float specularStrength;
uniform float shininess;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), shininess) * lightColor * objectColor;
    FragColor = vec4(specular, 1.0);
}
