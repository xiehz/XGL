#version 460 core

in vec3 viewpos;
layout(location = 0) out vec3 fragpos;

void main(){
	fragpos = viewpos;
}