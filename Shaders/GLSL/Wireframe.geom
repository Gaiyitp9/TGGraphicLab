#version 320 es
layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

layout(location = 0) in vec2 vTexCoord[];
layout(location = 1) in vec3 vColor[];
layout(location = 0) out vec2 fTexCoord;
layout(location = 1) out vec3 fColor;

void main()
{
   gl_Position = gl_in[0].gl_Position;
   fColor = vColor[0];
   fTexCoord = vTexCoord[0];
   EmitVertex();

   gl_Position = gl_in[1].gl_Position;
   fColor = vColor[1];
   fTexCoord = vTexCoord[1];
   EmitVertex();

   gl_Position = gl_in[2].gl_Position;
   fColor = vColor[2];
   fTexCoord = vTexCoord[2];
   EmitVertex();

   gl_Position = gl_in[0].gl_Position;
   fColor = vColor[0];
   fTexCoord = vTexCoord[0];
   EmitVertex();

   EndPrimitive();
}
