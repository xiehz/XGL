#version 330

layout (location = 0) in vec3 pos;

out vec2 texcoord;

void main(){

	vec4 clip = vec4(pos, 1.0);
	gl_Position = clip.xyww;

	texcoord = (pos.xy + vec2(1))/ 2.0;
}