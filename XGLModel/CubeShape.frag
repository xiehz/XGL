#version 460 core

in vec3 texcoord;

uniform samplerCube g_cubeSampler;

out vec4 fcolor;

void main(){

	fcolor =vec4(texture(g_cubeSampler, texcoord).r, 0.0, 0.0, 1.0);
}