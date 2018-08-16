#version 460 core

layout(location = 0) in vec3 pos;

out vec2 texcoord;

out vec2 viewray;

void main(){

	gl_Position = vec4(pos, 1.0);
	texcoord = (pos.xy + vec2(1))/ 2.0;

	//四边形的顶点光栅化插值，可理解为NDC
	viewray = pos.xy;
}