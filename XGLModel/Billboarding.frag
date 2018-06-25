#version 330

in vec2 texcoord;

uniform sampler2D g_sampler2d;

out vec4 color;
void main(){

	color = texture2D(g_sampler2d, texcoord);
	if(color.r == 0 && color.g == 0 && color.b == 0)
		discard;

}