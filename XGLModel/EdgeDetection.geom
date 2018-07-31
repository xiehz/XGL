#version 330

layout(triangles_adjacency) in ;
layout(line_strip, max_vertices = 6) out;

in vec3 worldPos[];

uniform vec3 g_lightPos;

void lineEmit(int st, int end)
{
	gl_Position = gl_in[st].gl_Position;
	EmitVertex();

	gl_Position= gl_in[end].gl_Position;
	EmitVertex();

	EndPrimitive();
}

void main(){
	
	vec3 e1 = worldPos[2] - worldPos[0];
    vec3 e2 = worldPos[4] - worldPos[0];
    vec3 e3 = worldPos[1] - worldPos[0];
    vec3 e4 = worldPos[3] - worldPos[2];
    vec3 e5 = worldPos[4] - worldPos[2];
    vec3 e6 = worldPos[5] - worldPos[0];
    vec3 Normal = cross(e1,e2);
    vec3 LightDir = g_lightPos - worldPos[0];
    if (dot(Normal, LightDir) > 0.00001) {
        Normal = cross(e3,e1);
        if (dot(Normal, LightDir) <= 0) {
            lineEmit(0, 2);
        }
        Normal = cross(e4,e5);
        LightDir = g_lightPos - worldPos[2];
        if (dot(Normal, LightDir) <=0) {
            lineEmit(2, 4);
        }
        Normal = cross(e2,e6);
        LightDir = g_lightPos - worldPos[4];
        if (dot(Normal, LightDir) <= 0) {
            lineEmit(4, 0);
        }
    }
	
}