#version 320 es
layout(points) in;
layout(lines, max_vertices = 256) out;

layout(location = 0) out vec3 fColor;

uniform mat4 view;
uniform mat4 projection;
uniform float size;
uniform float gridSpacing = 0.1;

void main()
{
    vec3 origin = gl_in[0].gl_Position.xyz;

    uint gridCount = uint(size / gridSpacing);
    float end = gridCount * gridSpacing;

    mat4 pv = projection * view;

    vec3 color = vec3(0.2);

    for (uint i = 0; i <= gridCount; ++i)
    {
        float x = i * gridSpacing;
        gl_Position = pv * vec4(origin + vec3(0, 0, x), 1.0);
        fColor = color;
        EmitVertex();
        gl_Position = pv * vec4(origin + vec3(end, 0, x), 1.0);
        fColor = color;
        EmitVertex();
        EndPrimitive();

        gl_Position = pv * vec4(origin + vec3(x, 0, 0), 1.0);
        fColor = color;
        EmitVertex();
        gl_Position = pv * vec4(origin + vec3(x, 0, end), 1.0);
        fColor = color;
        EmitVertex();
        EndPrimitive();
    }
}
