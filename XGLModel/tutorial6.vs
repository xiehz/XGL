#version 330 

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 mvp;

out vec2 texcoord0;

void main()
{
	vec4 clip = mvp *vec4(position ,1.0);
	gl_Position = mvp * clip;

	texcoord0 = texcoord;
}