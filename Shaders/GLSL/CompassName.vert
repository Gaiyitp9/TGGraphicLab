#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexcoord;

layout(location = 0) out vec2 vTexcoord;

out gl_PerVertex
{
    vec4 gl_Position;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vTexcoord = aTexcoord;
}
