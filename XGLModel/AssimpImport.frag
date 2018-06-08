#version 330

in vec2 texcoord0;
in vec3 normal0;

uniform sampler2D sampler2d;

out vec4 color;

void main()
{
	vec4 tex = texture2D(sampler2d, texcoord0);
	color = vec4(normalize(normal0),1.0) * tex ;
	color = tex;
}