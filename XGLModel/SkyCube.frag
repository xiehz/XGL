#version 330

in vec3 texcoord;

uniform samplerCube g_cubeSampler;

out vec4 fcolor;

void main(){

	fcolor = texture(g_cubeSampler, texcoord);
}