#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vclr;
uniform float fscale;
uniform mat4 modelview;
out vec4 color;

void main(){
	gl_Position = modelview* vec4(position.x, position.y*fscale,position.z,1.0);
 // gl_Position = vec4(position.x*sin(fscale), position.y*cos(fscale),position.z,1.0 );
  color = vclr;
}
