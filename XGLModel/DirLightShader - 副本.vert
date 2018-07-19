#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 g_mv;
uniform mat4 g_normal;
uniform mat4 g_mvp;

out vec3 epos;
out vec2 ftexcoord;
out vec3 enormal;

void main(){
	
	gl_Position = g_mvp * vec4(pos,1.0);

	epos = (g_mv *vec4(pos, 1.0)).xyz;
	ftexcoord = texcoord;
	enormal = (g_normal* vec4(normal,0.0)).xyz;
}
