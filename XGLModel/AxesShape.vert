#version 330

layout (location = 0 ) in vec3 pos;
layout (location = 1) in vec4 color;

uniform mat4 g_mv;
uniform mat4 g_pers;

out vec4 fragColor;

void main(){

	gl_Position = g_pers * g_mv * vec4(pos, 1.0);
	fragColor = color;
}