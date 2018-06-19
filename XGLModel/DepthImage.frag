#version 330

in vec2 texcoord;
in vec4 tnormal;

uniform sampler2D g_sampler2d;

out vec4 color;

void main()
{
	float depth = texture(g_sampler2d, texcoord).x;
	depth = 1.0 - (1.0 - depth)  * 25.0;

	color = vec4(depth);
//	color = texture2D(g_sampler2d, texcoord);
//	color = tnormal;
}