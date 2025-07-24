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

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - WorldPos);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * lightColor * objectColor;
    FragColor = vec4(diffuse, 1.0);
}
