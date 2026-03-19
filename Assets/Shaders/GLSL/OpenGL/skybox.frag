/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#version 410 core

layout(location = 0) in vec3 fTexCoord;

layout(location = 0) out vec4 FragColor;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, fTexCoord);
}
