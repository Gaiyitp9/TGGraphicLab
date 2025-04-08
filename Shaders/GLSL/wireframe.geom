#version 320 es
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

layout(location = 0) in vec3 vColor[];
layout(location = 0) out vec3 fColor;

void main()
{
   gl_Position = gl_in[0].gl_Position;
   fColor = vColor[0];
   EmitVertex();
   gl_Position = gl_in[1].gl_Position;
   fColor = vColor[1];
   EmitVertex();
   EndPrimitive();

   gl_Position = gl_in[1].gl_Position;
   fColor = vColor[1];
   EmitVertex();
   gl_Position = gl_in[2].gl_Position;
   fColor = vColor[2];
   EmitVertex();
   EndPrimitive();

   gl_Position = gl_in[2].gl_Position;
   fColor = vColor[2];
   EmitVertex();
   gl_Position = gl_in[0].gl_Position;
   fColor = vColor[0];
   EmitVertex();
   EndPrimitive();
}
