#version 330

in vec2 texcoord0;
uniform sampler2D sampler;
out vec4 color;

void main(){
	color = texture2D(sampler, texcoord0.st);	
}