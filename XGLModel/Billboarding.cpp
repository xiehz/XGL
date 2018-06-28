#include "stdafx.h"
#include "SkyCube.h"
#include "Billboarding.h"

namespace XGLModel {

	REGISTER(Billboarding)

}

XGLModel::Billboarding::Billboarding()
{
	m_pSky = 0;
	m_pTex = 0;
}


XGLModel::Billboarding::~Billboarding()
{
	if (m_pSky)
		delete m_pSky;
	m_pSky = 0;

	if (m_pTex)
		delete m_pTex;
	m_pTex = 0;
}

void XGLModel::Billboarding::initGL()
{
	m_pSky = dynamic_cast<SkyCube*>(TutorialFactory::getInstance().getClass("SkyCube"));
	m_pSky->bindHandle(getHandle());
	m_pSky->init();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	float vs[3] = { 0 };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);

	m_pTex = new XTexture(GL_TEXTURE_2D, "E:/2018/opengl/Assimp/data/monster_hellknight.png");
	m_pTex->Load();

}
void XGLModel::Billboarding::setBillboardSize(float BillboardSize)
{
	glUniform1f(g_billboardsize, BillboardSize);
}

void XGLModel::Billboarding::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	Matrixf& mv = camera->getInverseMatrix();
	const XGL::Vec3f& trans = mv.getTrans();

	glUseProgram(program);

	glUniform1f(g_billboardsize, 1.0f);
	glUniform3f(g_eye, trans.x(), trans.y(), trans.z());

	Matrixf transMat = Matrixf::identity();
	transMat.setTrans(trans);

	Matrixf test = projectMatrix;
	test.preMult(transMat);

	glUniformMatrix4fv(g_pers, 1, GL_FALSE, test.ptr());
	glUniform1i(g_sampler2d, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	m_pTex->Bind(GL_TEXTURE0);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableVertexAttribArray(0);

	glUseProgram(m_pSky->program);
	m_pSky->setCamera(camera->getInverseMatrix());
	m_pSky->render();
}

void XGLModel::Billboarding::initUniform()
{
	g_eye = glGetUniformLocation(program, "g_eye");
	g_pers = glGetUniformLocation(program, "g_vp");
	g_sampler2d = glGetUniformLocation(program, "g_sampler2d");
	g_billboardsize = glGetUniformLocation(program, "g_billboardsize");
	//getErrorInformation(GetLastError());
	if (g_eye < 0
		|| g_pers< 0
		|| g_sampler2d< 0
		|| g_billboardsize < 0)
	{
		XGLERROR("get uniform failed");
	}
}
