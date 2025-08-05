#version 420 core
layout(location = 0) in vec3 WorldPos;
layout(location = 1) in vec2 fTexcoord;
layout(locaiton = 2) in vec3 Normal;

layout(location = 0) out vec4 FragColor;

layout(std140, binding = 1) uniform RenderData
{
    vec3 lightDir;
    vec3 lightColor;
    vec3 viewPos;
};
layout(binding = 0) uniform sampler2D albedoMap;
uniform vec3 objectColor;

void main()
{
    vec3 albedo = texture(albedoMap, fTexcoord).rgb;
    vec3 normal = normalize(Normal);
//    vec3 lightDirNorm = normalize(lightPos - WorldPos);
    vec3 lightDirNorm = normalize(lightDir);
    vec3 diffuse = max(dot(normal, lightDirNorm), 0.0) * lightColor * objectColor * albedo;
    FragColor = vec4(diffuse, 1.0);
}
