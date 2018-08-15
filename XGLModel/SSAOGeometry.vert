#version 460 core

layout(location = 0) in vec3 pos;

uniform mat4 g_mv;
uniform mat4 g_mvp;

out vec3 viewpos;

void main(){
	viewpos = (g_mv * vec4(pos,1.0)).xyz;
	gl_Position = g_mvp* vec4(pos, 1.0);
}