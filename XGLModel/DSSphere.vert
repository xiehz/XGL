#version 330

layout(location = 0) in vec3 pos;

uniform mat4 g_mvp;

void main(){

	gl_Position = g_mvp* vec4(pos, 1.0);

}