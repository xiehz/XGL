#version 330

in vec2 texcoord;

uniform sampler2D g_sampler;

out vec4 fcolor;

void main(){

	fcolor = texture(g_sampler, texcoord);
}