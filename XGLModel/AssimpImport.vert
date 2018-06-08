#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

uniform mat4 mv;
uniform mat4 perspective;

out vec2 texcoord0;
out vec3 normal0;

void main(){

	gl_Position = perspective * mv * vec4(pos, 1.0);
	normal0 = (transpose(mv)*vec4(normal, 0.0)).xyz;
	texcoord0 = texcoord;
}
