#include "stdafx.h"
#include "XVAOMesh.h"
#include "SimpleVAO.h"

namespace XGLModel {

	REGISTER(SimpleVAO)

}
XGLModel::SimpleVAO::SimpleVAO()
{
	m_Mesh = 0;
	m_Mesh1 = 0;
}


XGLModel::SimpleVAO::~SimpleVAO()
{
	if (m_Mesh)
		delete m_Mesh;
	m_Mesh = 0;

	if (m_Mesh1)
		delete m_Mesh1;
	m_Mesh1 = 0;
}

void XGLModel::SimpleVAO::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	m_Mesh = new XVAOMesh();
	m_Mesh->LoadMesh("E:/2018/opengl/Assimp/data/phoenix_ugv.md2");

	m_Mesh1 = new XVAOMesh();
	m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");

}

void XGLModel::SimpleVAO::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	Matrixf& mv = camera->getInverseMatrix();

	glUniformMatrix4fv(g_mv, 1, GL_FALSE, mv.ptr());
	glUniformMatrix4fv(g_pers, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_sampler2d, 0);

	m_Mesh->Render();

	Matrixf trans = mv;
	trans.preMult(Matrixf::translate(0.0f, 40.0f, 0.0f));
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, trans.ptr());
	m_Mesh1->Render();
}

void XGLModel::SimpleVAO::initUniform()
{
	g_mv = glGetUniformLocation(program, "g_mv");
	g_pers = glGetUniformLocation(program, "g_pers");
	g_sampler2d = glGetUniformLocation(program, "g_sampler2d");
	//getErrorInformation(GetLastError());
	if (g_mv < 0
		|| g_pers< 0
		|| g_sampler2d< 0)
	{
		XGLERROR("get uniform failed");
	}
}
