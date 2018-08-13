#version 430 core


in vec3 worldp;

uniform vec3 g_eyeworld;

//RED ∏Ò Ω£¨ GL_R32F
out float color ;

void main()
{
	color =length( worldp - g_eyeworld ) / 10.0f;
}

