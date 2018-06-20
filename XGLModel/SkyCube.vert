#version 330

layout (location = 0) in vec3 pos;

uniform mat4 g_mv;
uniform mat4 g_pers;

out vec3 texcoord;

void main(){

	vec4 clip = g_pers * g_mv * vec4(pos,1.0);
	gl_Position = clip.xyww;
	texcoord = pos;
}