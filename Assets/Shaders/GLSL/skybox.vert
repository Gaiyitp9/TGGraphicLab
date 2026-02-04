#version 410 core
layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 vTexCoord;

uniform mat4 view;
uniform mat4 projection;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    vTexCoord = aPos;
    vec4 position = projection * view * vec4(aPos, 1.0);
    gl_Position = position.xyww;
}
