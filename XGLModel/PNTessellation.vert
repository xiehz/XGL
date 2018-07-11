
#version 410 core

layout(location = 0) in  vec3 in_vs_pos;
layout(location = 1) in vec2  in_vs_texcoord;
layout(location = 2) in vec3 in_vs_normal;


uniform mat4 g_world;

out vec3 in_cs_world;
out vec2 in_cs_texcoord;
out vec3 in_cs_wnormal;

void main(){
	
	in_cs_world = (g_world * vec4(in_vs_pos, 1.0)).xyz;
	in_cs_texcoord = in_vs_texcoord;
	in_cs_wnormal = normalize((g_world * vec4(in_vs_normal, 1.0)).xyz);

}