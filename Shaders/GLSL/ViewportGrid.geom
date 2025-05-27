#version 320 es
layout(points) in;
layout(line_strip, max_vertices = 64) out;

layout(location = 0) out vec3 fColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec2 origin;
uniform float size;
uniform float gridSpacing = 1;

void main()
{
    uint gridCount = uint(size / gridSpacing);
    float end = gridCount * gridSpacing;

    mat4 pv = projection * view;

    vec3 color = vec3(0.2);

    for (uint i = 0; i <= gridCount; ++i)
    {
        float x = i * gridSpacing;
        gl_Position = pv * vec4(origin.x, 0, origin.y + x, 1.0);
        fColor = color;
        EmitVertex();
        gl_Position = pv * vec4(origin.x + end, 0, origin.y + x, 1.0);
        fColor = color;
        EmitVertex();
        EndPrimitive();

        gl_Position = pv * vec4(origin.x + x, 0, origin.y, 1.0);
        fColor = color;
        EmitVertex();
        gl_Position = pv * vec4(origin.x + x, 0, origin.y + end, 1.0);
        fColor = color;
        EmitVertex();
        EndPrimitive();
    }
}
