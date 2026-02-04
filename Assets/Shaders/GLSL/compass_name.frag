#version 420 core
layout(location = 0) in vec2 fTexcoord;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D fontTexture;
uniform vec3 fontColor;

void main()
{
    FragColor = texture(fontTexture, fTexcoord).rrrr * vec4(fontColor, 1.0);
}
