#version 420 core
layout(location = 0) out vec4 FragColor;

layout (std140, binding = 1) uniform RenderData
{
    vec3 lightPos;
    vec3 lightColor;
    vec3 viewPos;
};
uniform vec3 objectColor;
uniform float ambientStrength;

void main()
{
    vec3 ambient = ambientStrength * lightColor * objectColor;
    FragColor = vec4(ambient, 1.0);
}
