#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexcoord;
layout(location = 2) in vec3 aNormal;

layout(location = 0) out vec3 WorldPos;
layout(location = 1) out vec2 vTexcoord;
layout(location = 2) out vec3 Normal;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(std140, binding = 0) uniform CameraData
{
    mat4 view;
    mat4 projection;
};
uniform mat4 model;

void main()
{
    WorldPos = vec3(model * vec4(aPos, 1.0));
    vTexcoord = aTexcoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
