#include "stdafx.h"
#include "Tutorial5.h"

using namespace XGLModel;
REGISTER(Tutorial5)
Tutorial5::Tutorial5()
{
}


Tutorial5::~Tutorial5()
{
}

void XGLModel::Tutorial5::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	float vertexAttribs[36] = { 
		0.0f,0.0f, 1.0f,
		1.0f,0.0f,0.0f,1.0f,

		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,1.0f,

		-1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,1.0f,

		0.0f,1.0f,0.0f,
		1.0f,1.0f,1.0f,1.0f,

		0.0f,-1.0f,-0.0f,
		1.0f,1.0f,0.0f,1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAttribs), vertexAttribs, GL_STATIC_DRAW);

	GLuint elements[18] = {
		1,3,0,
		1,2,3,
		0,3,2,
		1,0,4,
		1,4,2,
		0,2,4
	};
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	initShader();
}

void XGLModel::Tutorial5::draw()
{
	static float fscale = 0.3f;
	
	static float step = 0.0f;
	//step+= 0.1f;
	float deg = step*3.1415926f/180.0f;
	float model[16] = {

		cosf(deg),sinf(deg),0.0f,0.0f,
		-sinf(deg),cos(deg),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUniform1f(uniform_scale, fscale);
	/*
	It indicates whether the matrix is supplied in row-major or column-major order. 
	Row-major means the matrix is supplied row after row, starting from the top. Column-major is the same but in columns. 
	The point is that C/C++ are row-major languages by default. 
	This means that when you populate a two dimentional array with values they are laid out in memory row after row with the "top" row at the lower address
	
	So our third parameter to glUniformMatrix4fv() is GL_TRUE 
	because we supply the matrix in row-major order.
	We can also make the third parameter GL_FALSE 
	but then we will need to transpose the matrix values 
	(the C/C++ memory layout will remain the same but OpenGL will "think" that the first 4 values we supply are actually a matrix column and so on and will behave accordingly).
	
	c++�����������ڴ��еĴ洢��ʽ��
	row major
	m0 m1 m2 m3
	m4 m5 m6 m7
	m8 m9 m10 m11
	m12 m13 m14 m15

	opengl�������ڴ��еĴ洢��ʽ��
	column major
	m0    m4   	 m8   m12
	m1	  m5		m9   m13
	m2	  m6		m10  m14
	m3 	  m7		m11  m15

	��ѧ�����ϵľ�����row major ��column major �޹�
	mat4
	A*B
	��һ�����һ��������

	c++���鴫��opengl
	����Ҫת�ã����ܱ�֤�õ���ͬ����˷������
	*/
	glUniformMatrix4fv(uniform_model_matrix, 1,true,model);
	//����������
	glUniformMatrix4fv(uniform_view_matrix, 1, true, cameraMatrix.get());

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, (GLvoid*)(sizeof(GL_FLOAT)*3));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
}

void XGLModel::Tutorial5::initShader()
{
	program = glCreateProgram();
	if (!program)
	{
		XGLERROR("create program failed!");
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	if (!vs)
	{
		XGLERROR("create vertex shader failed!");
	}
	readShader("../../XGLModel/tutorial5.vs", vsSource);
	addShader(program,vs,vsSource);
	postViewMsg(1, vsSource);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fs)
	{
		XGLERROR("create fragment shader failed!");
	}
	readShader("../../XGLModel/tutorial5.fs", fsSource);
	addShader(program, fs, fsSource);
	postViewMsg(4, fsSource);


	linkProgram();

	uniform_scale = glGetUniformLocation(program, "fscale");
	if (uniform_scale < 0)
	{
		XGLERROR("get uniform failed");
	}

	uniform_model_matrix = glGetUniformLocation(program, "model");
	if (uniform_model_matrix < 0)
	{
		XGLERROR("get model uniform failed");
	}

	uniform_view_matrix = glGetUniformLocation(program, "view");
	if (uniform_view_matrix < 0)
	{
		XGLERROR("get view uniform failed");
	}
	validateProgram();

	glUseProgram(program);
}
