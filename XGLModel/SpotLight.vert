#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tc;
layout(location = 2) in vec3 n;

uniform mat4 g_mv;
uniform mat4 g_pers;


out vec2 tc0;
out vec3 tn;
out vec3 eyep;

void main(){
	
	gl_Position = g_pers * g_mv * vec4(pos,1.0);
	tn = (transpose(g_mv) * vec4(n,0.0)).xyz;
	tc0 = tc;
	eyep = (g_mv * vec4(pos,1.0)).xyz;
}