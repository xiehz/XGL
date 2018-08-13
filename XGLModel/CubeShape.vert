#version 460 core

layout (location = 0) in vec3 pos;

uniform mat4 g_world;
uniform mat4 g_vp;

out vec3 texcoord;

void main(){

	vec4 world = g_world * vec4(pos, 1.0);
	texcoord = normalize(world.xyz);

	gl_Position = g_vp * world;
}