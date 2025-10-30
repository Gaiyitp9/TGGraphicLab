#version 410 core
layout(points) in;
layout(line_strip, max_vertices = 128) out;

uniform mat4 view;
uniform mat4 projection;
uniform ivec2 origin;
uniform vec3 eyePosition;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(location = 0) out float gridAlpha;

void outputVertex(vec2 position, mat4 pv)
{
    gl_Position = pv * vec4(position.x, 0, position.y, 1.0);
    gridAlpha = (1.0 - distance(position, vec2(eyePosition.x, eyePosition.z)) / 80.0) * 0.2;
}

void main()
{
    int gridSpacing = 1;
    int size = 31;
    int gridCount = size / gridSpacing;

    mat4 pv = projection * view;

    for (int i = 0; i <= gridCount; ++i)
    {
        int d = i * gridSpacing;

        outputVertex(vec2(origin.x, origin.y + d), pv);
        EmitVertex();
        outputVertex(vec2(origin.x + size, origin.y + d), pv);
        EmitVertex();
        EndPrimitive();

        outputVertex(vec2(origin.x + d, origin.y), pv);
        EmitVertex();
        outputVertex(vec2(origin.x + d, origin.y + size), pv);
        EmitVertex();
        EndPrimitive();
    }
}
