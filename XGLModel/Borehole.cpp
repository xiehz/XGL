#include "stdafx.h"
#include "BoreoheMesh.h"
#include "Background.h"
#include "Borehole.h"

namespace XGLModel {

	REGISTER(Borehole)

}

XGLModel::Borehole::Borehole():N(1024 * 1024)
{
	m_Mesh = 0;
	m_Instances = 0;
	m_pSky = 0;
}


XGLModel::Borehole::~Borehole()
{
	if (m_Mesh)
		delete m_Mesh;
	m_Mesh = 0;
	if (m_Instances)
		delete[] m_Instances;
	m_Instances = 0;
	if (m_pSky)
		delete m_pSky;
	m_pSky = 0;
}

void XGLModel::Borehole::initGL()
{
	m_pSky = dynamic_cast<Background*>(TutorialFactory::getInstance().getClass("Background"));
	m_pSky->bindHandle(getHandle());
	m_pSky->init();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	//m_Mesh = new XVAOMesh();
	//m_Mesh->LoadMesh("E:/2018/opengl/Assimp/data/phoenix_ugv.md2");

	m_Mesh = new BoreoheMesh();
	m_Mesh->LoadMesh(12,1.0);

	m_Instances = new  float[N][16];
	const int row = 1024;
	const int col = N / row;

	Matrixf m;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			m = Matrixf::translate(-1024 + i*2 , 0, -1024 + j*2 );
			//m = Matrixf::translate(-10.24 + i * 0.02, 0,-10.24 + j * 0.02);
			m.preMult(Matrixf::scale(0.1, 10.0, 0.1));
			memcpy(m_Instances[i* col + j] , m.ptr(),sizeof(float)*16);
		}
	}
}

void XGLModel::Borehole::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glUseProgram(program);
	Matrixf mvp = projectMatrix;
	mvp.preMult(camera->getInverseMatrix());

	glUniformMatrix4fv(g_mvp, 1, GL_FALSE, mvp.ptr());

	//m_Mesh->Render();

	m_Mesh->Render(N, *m_Instances);

	glUseProgram(m_pSky->program);
	m_pSky->render();
}

void XGLModel::Borehole::initUniform()
{
	g_mvp = glGetUniformLocation(program, "g_mvp");
	//getErrorInformation(GetLastError());
	if (g_mvp < 0)
	{
		XGLERROR("get uniform failed");
	}
}
