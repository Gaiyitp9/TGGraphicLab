#version 320 es
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vTexCoord;

uniform mat4 transform;

void main()
{
   gl_Position = transform * vec4(aPos, 1.0);
   vColor = aColor;
   vTexCoord = aTexCoord;
}
