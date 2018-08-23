#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in mat4 world;

uniform mat4 g_mvp;
flat out int  instance;
out vec3 wnormal;
void main(){

	gl_Position = g_mvp * world* vec4(pos, 1.0);
	wnormal = (transpose(inverse(world))* vec4(normal, 0)).xyz;
	instance = gl_InstanceID;
}