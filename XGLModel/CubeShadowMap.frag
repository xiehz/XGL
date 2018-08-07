#version 330


in vec3 eyep;

//RED ∏Ò Ω£¨ GL_R32F
out float color ;

void main()
{
	color = length(eyep);
}

