/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#version 410 core

layout(location = 0) in float gridAlpha;

layout(location = 0) out vec4 FragColor;

uniform vec3 gridColor;

void main()
{
    FragColor = vec4(gridColor, gridAlpha);
}
