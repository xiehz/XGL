#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in float depth;


out Borehole{
	float depth;
	int instance;
}borehole;

out int instance;
void main(){

	gl_Position = vec4(pos, 1.0);

	borehole.depth = depth; 
	borehole.instance = gl_VertexID;
}