#include "stdafx.h"
#include "XGL\Vectors.h"
#include "Tutorial2.h"

using namespace XGLModel;
REGISTER(Tutorial2)
float vertexAttribs[7]{ 0.0f,0.0f,0.0f,1.0f,0.0f,0.0f ,1.0f };
float vertices[3]{ 0.0f,0.0f,0.0f };
float colors[4]{ 1.0f,0.0f,0.0f,1.0f };
unsigned int listID;
Tutorial2::Tutorial2()
{
}


Tutorial2::~Tutorial2()
{
}

void Tutorial2::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1);

	listID = glGenLists(1);
	glNewList(listID,GL_COMPILE);
	drawByBeginEnd();
	glEndList();

	createVertexBuffer();
}

void Tutorial2::draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectMatrix.get());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(cameraMatrix.get());

	glDisable(GL_LIGHTING);

	glPointSize(100.f);

	//vbo + glVertexAttriPointer的方式 + glEnableVetexAttribArray(index) -- index 是shader变量和应用程序变量映射
	glPushMatrix();
	glTranslatef(0.5f, 0.5f, -0.f);
	drawByVBO();
	glPopMatrix();

	//立即模式
	glPushMatrix();
	glTranslatef(-0.5f, 0.5f, -0.f);
	drawByBeginEnd();
	glPopMatrix();

	//显示列表
	glPushMatrix();
	glTranslatef(-0.5f, -0.5f, -0.f);
	drawByCalllise();
	glPopMatrix();

	//glEnableClientState + VBO  固定管线
	glPushMatrix();
	glTranslatef(0.5f, -0.5f, -0.f);
	drawByEnableClientStateVBO();

	glPopMatrix();

	//glEnableClientState +顶点数组 
	drawByEnableClientState();
	glEnable(GL_LIGHTING);

}

void XGLModel::Tutorial2::createVertexBuffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAttribs), vertexAttribs, GL_STATIC_DRAW);
}

void XGLModel::Tutorial2::drawByEnableClientState()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0, colors);

	glDrawArrays(GL_POINTS, 0, 1);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void XGLModel::Tutorial2::drawByEnableClientStateVBO()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_FLOAT, sizeof(GL_FLOAT) * 7, 0);
	glColorPointer(4, GL_FLOAT, sizeof(GL_FLOAT) * 7, (GLvoid*)(sizeof(GL_FLOAT) * 3));

	glDrawArrays(GL_POINTS, 0, 1);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void XGLModel::Tutorial2::drawByBeginEnd()
{
	glBegin(GL_POINTS);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glEnd();
}

void XGLModel::Tutorial2::drawByCalllise()
{
	glCallList(listID);
}

void XGLModel::Tutorial2::drawByVBO()
{
	//binding between the data in the C / C++ program and the attribute name inside the shader
	//shader:
	//1. shader中的vertex attributes 都有一个索引对应，用来支持创建一个应用程序的数据和shader中attribute name的绑定，
	//2.并且，您还必须启用每个vertex attribute的索引。
	//固定管线：
	//顶点加载到buffer中，并把vertex attribute索引当做0，
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//参数1： vertex attribute对应的索引；
	//参数2： 分量的个数；
	//3：数据类型
	//4：是否单位化
	//5：stride间隔，buffer中该attribute两个实例之间的byte字节数
	//如果是只有一个属性，且紧凑排列，那么stride =0
	//如果是包含顶点、法线的结构体，结构体大小 3*4 +3*4 = 24；
	//-------那么顶点之间的offset就是0， 法线之间的offset就是12-----------；
	glEnableVertexAttribArray(0);

	glBindBuffer( GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(vertexAttribs), 0);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);
}

void XGLModel::Tutorial2::initShader()
{
}







