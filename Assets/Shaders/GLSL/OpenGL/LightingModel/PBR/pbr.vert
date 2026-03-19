/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#version 420 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;

layout(std140, binding = 0) uniform CameraData
{
    mat4 view;
    mat4 projection;
};
uniform mat4 model;

out gl_PerVertex
{
    vec4 gl_Position;
};

out VS_OUT
{
    vec3 WorldPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vs_out.WorldPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoord = aTexCoord;

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(transpose(inverse(model)) * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    vs_out.TBN = mat3(T, B, N);
}
