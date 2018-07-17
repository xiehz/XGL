#version 330

in vec3 wpos;
in vec3 wnormal;
in vec2 ftexcoord;


uniform sampler2D g_sampler2d;

layout(location = 0 ) out vec3 fwordpos;
layout(location = 1 ) out vec3 fdiffuse;
layout(location = 2 ) out vec3 fnormal;
layout(location = 3 ) out vec3 ftexc;

void main(){
	fwordpos = wpos;
	fnormal = normalize(wnormal);
	ftexc = vec3(ftexcoord, 0);
	fdiffuse = texture(g_sampler2d, ftexcoord).xyz;
}