#version 330

layout(location = 0) in vec3 position;
out vec4 positionColor;

void main(){
	gl_Position = vec4(position,1.0);
	positionColor = vec4(cos(gl_Position.x),cos(gl_Position.y),sin(gl_Position.z),1.0);
}