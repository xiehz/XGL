#version 330

layout( location  = 0) in vec3 pos;
layout (location  = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

uniform mat4 g_world;
uniform mat4 g_vp;

out vec3 worldn;
out vec2 otex;
out vec3 worldp;

void main(){
	worldn = ( g_world * vec4(normal, 0.0)).xyz;
	vec4 wpos = (g_world * vec4(pos, 1.0));

	worldp = wpos.xyz;
	gl_Position = g_vp * wpos;
	otex = texcoord;
}
