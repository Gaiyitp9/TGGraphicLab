#version 320 es
layout(points) in;
layout(line_strip, max_vertices = 256) out;

uniform mat4 view;
uniform mat4 projection;
uniform ivec2 origin;

void main()
{
    int gridSpacing = 1;
    int size = 63;
    int gridCount = size / gridSpacing;

    mat4 pv = projection * view;

    for (int i = 0; i <= gridCount; ++i)
    {
        int x = i * gridSpacing;
        gl_Position = pv * vec4(origin.x, 0, origin.y + x, 1.0);
        EmitVertex();
        gl_Position = pv * vec4(origin.x + size, 0, origin.y + x, 1.0);
        EmitVertex();
        EndPrimitive();

        gl_Position = pv * vec4(origin.x + x, 0, origin.y, 1.0);
        EmitVertex();
        gl_Position = pv * vec4(origin.x + x, 0, origin.y + size, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}
