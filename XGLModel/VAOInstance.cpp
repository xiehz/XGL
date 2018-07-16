#include "stdafx.h"
#include "XVAOMesh.h"
#include "VAOInstance.h"

namespace XGLModel {

	REGISTER(VAOInstance)

}

XGLModel::VAOInstance::VAOInstance():N(1024)
{
	m_Mesh = 0;
	m_Mesh1 = 0;
	m_Instances = 0;
}


XGLModel::VAOInstance::~VAOInstance()
{
	if (m_Mesh)
		delete m_Mesh;
	m_Mesh = 0;

	if (m_Mesh1)
		delete m_Mesh1;
	m_Mesh1 = 0;
	if (m_Instances)
		delete[] m_Instances;
	m_Instances = 0;
}

void XGLModel::VAOInstance::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	//m_Mesh = new XVAOMesh();
	//m_Mesh->LoadMesh("E:/2018/opengl/Assimp/data/phoenix_ugv.md2");

	m_Mesh1 = new XVAOMesh();
	m_Mesh1->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");

	m_Instances = new  float[N][16];
	const int row = 32;
	const int col = N / row;

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			memcpy(m_Instances[i* col + j] , Matrixf::translate(-128 + i * 8, 0, -128 + j*8).ptr(),sizeof(float)*16);
		}
	}
}

void XGLModel::VAOInstance::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	Matrixf mvp = projectMatrix;
	mvp.preMult(camera->getInverseMatrix());

	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());
	glUniform1i(g_sampler2d, 0);

	//m_Mesh->Render();

	m_Mesh1->Render(N, *m_Instances);
}

void XGLModel::VAOInstance::initUniform()
{
	g_mvp = glGetUniformLocation(program, "g_mvp");
	g_sampler2d = glGetUniformLocation(program, "g_sampler2d");
	//getErrorInformation(GetLastError());
	if (g_mvp < 0
		|| g_sampler2d< 0)
	{
		XGLERROR("get uniform failed");
	}
}
