#version 330

layout(location = 0) in vec3 position;
out vec4 positionColor;

void main(){
	gl_Position = position;
	positionColor = vec4(cos(gl_Position.x),cos(gl_Position.y),sin(gl_Position.z),1.0);
}