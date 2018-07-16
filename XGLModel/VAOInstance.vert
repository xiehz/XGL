#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in mat4 world;

uniform mat4 g_mvp;
out vec2 fs_texcoord;
flat out int  instance;
void main(){

	gl_Position = g_mvp * world* vec4(pos, 1.0);
	fs_texcoord = texcoord;
	instance = gl_InstanceID;
}