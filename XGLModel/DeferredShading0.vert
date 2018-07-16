#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 g_world;
uniform mat4 g_vp;


out vec3 wpos;
out vec3 wnormal;
out vec2 ftexcoord;



void main(){

	gl_Position = g_vp* g_world* vec4(pos, 1.0);
	wpos = (g_world * vec4(pos, 1.0)).xyz;
	wnormal = (g_world * vec4(normal, 1.0)).xyz;
	ftexcoord = texcoord;
}