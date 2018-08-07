#version 330

layout( location  = 0) in vec3 pos;
layout (location  = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

uniform mat4 g_mv;
uniform mat4 g_pers;

out vec3 eyep;

void main(){
	gl_Position = g_pers * g_mv * vec4(pos, 1.0);
	eyep =( g_mv* vec4(pos, 1.0)).xyz;
}
