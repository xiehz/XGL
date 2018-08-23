#include "stdafx.h"
#include "XVAOMesh.h"
#include "Background.h"
#include "BoreholeGeometry.h"

namespace XGLModel {

	REGISTER(BoreholeGeometry)

}

XGLModel::BoreholeGeometry::BoreholeGeometry():N(1024*1024)
{
	m_Instances = 0;
	m_pSky = 0;
}


XGLModel::BoreholeGeometry::~BoreholeGeometry()
{
	if (m_Instances)
		delete[] m_Instances;
	m_Instances = 0;
	if (m_pSky)
		delete m_pSky;
}

void XGLModel::BoreholeGeometry::initGL()
{

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);


	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &m_DepthBuffer);

	m_Instances = new  float[N][3];
	const int row = 1024;
	const int col = N / row;
	float* depths = new float[N];

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			m_Instances[i*col + j][0] = -4096 + i * 8;
			m_Instances[i*col + j][1] = 0;
			m_Instances[i*col + j][2] = -4096 + j * 8;
			//m_Instances[i*col + j][0] = 0;
			//m_Instances[i*col + j][1] = 0;
			//m_Instances[i*col + j][2] = 0;
			depths[i * col + j] = rand() % (100 -10) + 10.f;

		}
	}


	// Generate and populate the buffers with vertex attributes and the indices

	int pos_location = 0;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,N * sizeof(float)*3, m_Instances, GL_STATIC_DRAW);
	glEnableVertexAttribArray(pos_location);
	glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//float (*p)[1024] = new float[][1024];
	//float* p[1024] = {0};

	glBindBuffer(GL_ARRAY_BUFFER, m_DepthBuffer);
	glBufferData(GL_ARRAY_BUFFER, N * sizeof(float) , depths, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	delete[] depths;
	depths = 0;

	m_pTex = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/timg.jpg");
	m_pTex->Load();

	m_pSky = dynamic_cast<Background*>(TutorialFactory::getInstance().getClass("Background"));
	m_pSky->bindHandle(getHandle());
	m_pSky->init();
}

void XGLModel::BoreholeGeometry::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	Matrixf mvp = projectMatrix;
	mvp.preMult(camera->getInverseMatrix());
	glUseProgram(program);

	glBindVertexArray(m_VAO);
	glUniformMatrix4fv(g_vp, 1, GL_FALSE, mvp.ptr());
	m_pTex->Bind(GL_TEXTURE0);
	glUniform1i(g_sampler, 0);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Count);
	glDrawArrays(GL_POINTS, 0, N);

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);

	glUseProgram(m_pSky->program);
	m_pSky->render();
}

void XGLModel::BoreholeGeometry::initUniform()
{
	g_vp = glGetUniformLocation(program, "g_vp");
	g_sampler = glGetUniformLocation(program, "g_sampler");
	//getErrorInformation(GetLastError());
	if (g_vp < 0
		|| g_sampler < 0)
	{
		XGLERROR("get uniform failed");
	}
}
