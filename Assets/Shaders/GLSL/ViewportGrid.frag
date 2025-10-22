#version 410 core
layout(location = 0) out vec4 FragColor;

uniform vec4 gridColor;

void main()
{
    FragColor = gridColor;
}
