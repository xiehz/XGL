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

void XGLModel::Tutorial5::initGL()
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
}

void XGLModel::Tutorial5::draw()
{
	static float fscale = 1.0f;
	
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
	
	c++数组在连续内存中的存储方式：
	row major
	m0 m1 m2 m3
	m4 m5 m6 m7
	m8 m9 m10 m11
	m12 m13 m14 m15

	opengl数组在内存中的存储方式：
	column major
	m0    m4   	 m8   m12
	m1	  m5		m9   m13
	m2	  m6		m10  m14
	m3 	  m7		m11  m15

	数学意义上的矩阵与row major 和column major 无关
	mat4
	A*B
	第一行与第一列相乘相加

	c++数组传至opengl
	则需要转置，才能保证得到相同矩阵乘法结果。
	*/
	glUniformMatrix4fv(uniform_model_matrix, 1,true,model);
	//相机矩阵的逆
	float m[4][4];
	m[0][0] = 1.0;	m[0][1] = 0.0;	m[0][2] = 0.0;	m[0][3] = 0.0;
	m[1][0] = 0.0;	m[1][1] = 1.0;	m[1][2] = 0.0;	m[1][3] = 0.0;
	m[2][0] = 0.0;	m[2][1] = 0.0;	m[2][2] = 1.0;	m[2][3] = 0.0;
	m[3][0] = 0.3;	m[3][1] = 1.0;	m[3][2] = -2.0;	m[3][3] = 1.0;
	
	Matrix& cameraMatrix = camera->getInverseMatrix();


	////-------------------------
	//XGL::Vec3f Target(0.0f, 0.0f, 0.0f);
	//XGL::Vec3f Eposition(2.0f, 1.0f, 0.0f);
	//XGL::Vec3f Direction = Eposition - Target;

	//XGL::Vec3f right, up, forward;
	//forward = Direction;
	//up = Vec3f(0.0f, 1.0f, 0.0f);

	//right = up ^ forward;
	//right.normalize();

	//up = forward ^ right;

	//float lightView[4][4] = {
	//	right.x(), up.x(), forward.x(),0,
	//	right.y(), up.y(), forward.y(),0,
	//	right.z(), up.z(), forward.z(),0,
	//	-Eposition * right , -Eposition * up, -Eposition * forward,1,
	//};

	glUniformMatrix4fv(uniform_view_matrix, 1, false, cameraMatrix.ptr());
	glUniformMatrix4fv(uniform_perspective, 1, false, projectMatrix.ptr());

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

void XGLModel::Tutorial5::initUniform()
{
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

	uniform_perspective = glGetUniformLocation(program, "perspective");
	if (uniform_perspective < 0)
	{
		XGLERROR("get perspective failed");
	}
}


