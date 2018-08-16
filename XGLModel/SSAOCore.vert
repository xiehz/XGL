#version 460 core

layout(location = 0) in vec3 pos;

out vec2 texcoord;

out vec2 viewray;

void main(){

	gl_Position = vec4(pos, 1.0);
	texcoord = (pos.xy + vec2(1))/ 2.0;

	//�ı��εĶ����դ����ֵ�������ΪNDC
	viewray = pos.xy;
}