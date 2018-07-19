#version 330

in vec3 epos;
in vec3 enormal;
in vec2 ftexcoord;


uniform sampler2D g_sampler2d;

layout(location = 0 ) out vec3 fepos;
layout(location = 1 ) out vec3 fdiffuse;
layout(location = 2 ) out vec3 fenormal;
layout(location = 3 ) out vec3 ftexc;

void main(){
	fepos = epos;
	fenormal = normalize(enormal);
	ftexc = vec3(ftexcoord, 0);
	fdiffuse = texture(g_sampler2d, ftexcoord).xyz;
}