#include "stdafx.h"
#include "Tutorial3.h"

using namespace XGLModel;

REGISTER(Tutorial3)

Tutorial3::Tutorial3()
{
}


Tutorial3::~Tutorial3()
{
}

void XGLModel::Tutorial3::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float vertices[21]{
		-1.0f,-1.0f,0.0f,
		1.0f,0.0f,0.0f,1.0f,

	1.0f,-1.0f,0.0f,
	0.0f,1.0f,0.0f,1.0f,

	0.0f,1.0f,0.0f,
	0.0f,0.0f,1.0f,1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void XGLModel::Tutorial3::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectMatrix.ptr());

	Matrix cameraMatrix = camera->getInverseMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraMatrix.ptr());


	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	//glGetIntegerv(GL_MODELVIEW, );
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(GL_FLOAT)*7, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, sizeof(GL_FLOAT) * 7, (GLvoid*)(sizeof(GL_FLOAT) *3));
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	//drawFixedPipeline();
}


void XGLModel::Tutorial3::initUniform()
{
}

void XGLModel::Tutorial3::initShader()
{
}

void XGLModel::Tutorial3::drawFixedPipeline()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexPointer(3, GL_FLOAT, sizeof(GL_FLOAT) * 7, 0);
	glColorPointer(4, GL_FLOAT, sizeof(GL_FLOAT) * 7, (GLvoid*)(sizeof(GL_FLOAT) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

}







