#version 330


in vec2 fs_texcoord;
in vec3 fs_normal;

uniform sampler2D g_sampler2d;

out vec4 color;
void main(){
	color = texture2D(g_sampler2d, fs_texcoord);
}