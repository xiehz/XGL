#version 330

in vec2 texcoord0;
in vec3 normal0;

uniform sampler2D sampler2d;

out vec4 color;

void main()
{
	float depth = texture(sampler2d, texcoord0).x;
	depth = 1.0 - (1.0 - depth)  * 25.0;
	color = vec4(depth);
}