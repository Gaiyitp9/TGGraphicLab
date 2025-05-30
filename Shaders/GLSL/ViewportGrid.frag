#version 320 es
precision mediump float;

layout(location = 0) out vec4 FragColor;

uniform vec3 gridColor;

void main()
{
    FragColor = vec4(gridColor, 1.0);
}
