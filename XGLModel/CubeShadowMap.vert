#version 430 core

layout( location  = 0) in vec3 pos;
layout (location  = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

uniform mat4 g_world;
uniform mat4 g_vp;

out vec3 worldp;

void main(){
	vec4 world = g_world * vec4(pos,1.0);

	worldp = world.xyz;

	gl_Position = g_vp * world;
}
