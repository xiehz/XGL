#version 430 core


in vec2 texcoord;
in vec4 tnormal;

uniform sampler2D g_sampler2d;

out vec4 color;

void main(){

	color = normalize(tnormal) ; 	
}