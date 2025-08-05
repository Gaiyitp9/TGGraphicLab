#version 420 core
layout(location = 1) in vec2 fTexcoord;

layout(location = 0) out vec4 FragColor;

layout(std140, binding = 1) uniform RenderData
{
    vec3 lightDir;
    vec3 lightColor;
    vec3 viewPos;
};
layout(binding = 0) uniform sampler2D albedoMap;
uniform vec3 objectColor;
uniform float ambientStrength;

void main()
{
    vec3 albedo = texture(albedoMap, fTexcoord).rgb;
    vec3 ambient = ambientStrength * lightColor * objectColor * albedo;
    FragColor = vec4(ambient, 1.0);
}
