#version 320 es
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

layout(location = 0) in vec3 vColor[];
layout(location = 1) in vec2 vTexCoord[];
layout(location = 0) out vec3 fColor;
layout(location = 1) out vec2 fTexCoord;

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
   EndPrimitive();

   gl_Position = gl_in[1].gl_Position;
   fColor = vColor[1];
   fTexCoord = vTexCoord[1];
   EmitVertex();
   gl_Position = gl_in[2].gl_Position;
   fColor = vColor[2];
   fTexCoord = vTexCoord[2];
   EmitVertex();
   EndPrimitive();

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
