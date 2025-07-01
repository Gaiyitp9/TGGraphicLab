#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aColor;

layout(location = 0) out vec2 vTexCoord;
layout(location = 1) out vec3 vColor;

uniform mat4 transform;

out gl_PerVertex
{
   vec4 gl_Position;
};

void main()
{
   gl_Position = transform * vec4(aPos, 1.0);
   vColor = aColor;
   vTexCoord = aTexCoord;
}
