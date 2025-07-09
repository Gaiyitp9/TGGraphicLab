#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 vColor;

uniform mat4 view;
uniform mat4 projection;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    vColor = aColor;
}
