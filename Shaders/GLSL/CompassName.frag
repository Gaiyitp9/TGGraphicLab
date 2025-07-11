#version 410 core
layout(location = 0) in vec2 fTexcoord;

layout(location = 0) out vec4 FragColor;

uniform sampler2D fontTexture;
uniform vec3 fontColor;

void main()
{
    FragColor = texture(fontTexture, fTexcoord) * vec4(fontColor, 1.0);
}
