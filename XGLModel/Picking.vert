#version 330

layout( location = 0) in vec3 pos;

uniform mat4 g_mv;
uniform mat4 g_pers;

void main(){

	gl_Position =g_pers *  g_mv *  vec4(pos, 1.0);
}