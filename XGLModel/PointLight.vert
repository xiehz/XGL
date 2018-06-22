#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tc;
layout(location = 2) in vec3 n;

uniform mat4 mv;
uniform mat4 perspective;


out vec2 tc0;
out vec3 tn;
out vec3 eyep;

void main(){
	
	gl_Position = perspective * mv * vec4(pos,1.0);
	tn = (transpose(inverse(mv)) * vec4(n,0.0)).xyz;
	tc0 = tc;
	eyep = (mv * vec4(pos,1.0)).xyz;
}