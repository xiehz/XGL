#version 330

layout( location  = 0) in vec3 pos;
layout (location  = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

uniform mat4 g_mv;
uniform mat4 g_pers;
uniform mat4 g_lv;

out vec3 tnormal;
out vec2 otex;
out vec3 eyep;

void main(){
	gl_Position = g_pers * g_mv * vec4(pos, 1.0);
	tnormal = ( transpose(g_mv)* vec4(normal, 1.0)).xyz;
	otex = texcoord;
	eyep =( g_mv* vec4(pos, 1.0)).xyz;
}
