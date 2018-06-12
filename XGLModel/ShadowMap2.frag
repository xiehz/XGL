#version 330

in vec3 tnormal;
in vec2 otex;

uniform sampler2D g_sampler2d;

out vec4 color ;

void main()
{
	float depth = texture(g_sampler2d, otex).x;
	depth = 1.0 - (1.0 - depth)  * 25.0;
	color = vec4(depth);
}