#version 330


in vec3 eyep;

//RED ��ʽ�� GL_R32F
out float color ;

void main()
{
	color = length(eyep);
}

