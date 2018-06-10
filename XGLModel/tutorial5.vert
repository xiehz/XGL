#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vclr;
uniform float fscale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
out vec4 color;

void main(){
	mat4 modelview = view*model;
	mat4 clip = perspective*modelview ;
	gl_Position = clip* vec4(position.x*fscale, position.y*fscale,position.z*fscale,1.0);
  //gl_Position = vec4(position.x*sin(fscale), position.y*cos(fscale),position.z,1.0 );
  color = clamp(vclr,0.0,1.0);

}
