#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 color;

uniform mat4 mvp;
out vec2 texcoord0;
out vec4 color0;

void main()
{
	gl_Position = mvp * vec4(pos,1.0);

	texcoord0 = texcoord;
	color0 = color;
}