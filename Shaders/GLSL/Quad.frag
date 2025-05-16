#version 320 es
precision mediump float;

layout(location = 0) in vec2 fTexCoord;
layout(location = 1) in vec3 fColor;
layout(location = 0) out vec4 FragColor;

uniform vec4 ourColor;
uniform sampler2D albedo0;
uniform sampler2D albedo1;

void main()
{
	FragColor = mix(texture(albedo0, fTexCoord) * texture(albedo1, fTexCoord),
					vec4(fColor, 1.0f) * ourColor, 0.5);
}
