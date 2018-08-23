#include "stdafx.h"
#include "Background.h"
#include "IXMesh.h"
#include "Explode.h"
#include  "AssimpImport.h"

namespace XGLModel {

	REGISTER(Explode)

}

XGLModel::Explode::Explode()
{
	m_layer = 0;
	m_inner = 0;
	m_pSky = 0;
}


XGLModel::Explode::~Explode()
{
	if (m_layer)
		delete m_layer;
	m_layer = 0;

	if (m_inner)
		delete m_inner;
	m_inner = 0;

	if (m_pSky)
		delete m_pSky;
}

void XGLModel::Explode::initGL()
{
	m_layer = new IXMesh();
	m_layer->LoadMesh("E:/2018/opengl/Assimp/data/closed.obj");

	m_inner = new AssimpImport();
	m_inner->setName("AssimpImport");
	m_inner->initShader();

	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	m_pSky = dynamic_cast<Background*>(TutorialFactory::getInstance().getClass("Background"));
	m_pSky->bindHandle(getHandle());
	m_pSky->init();
}
void XGLModel::Explode::setExploder(float ex)
{
	glUniform1f(g_exploder, ex);
}

void XGLModel::Explode::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glUseProgram(m_inner->program);
	Matrixf mv = camera->getInverseMatrix();
	mv.preMult(Matrixf::translate(0,0, -50));
	mv.preMult(Matrixf::scale(0.5f, 0.5f, 0.5f));

	glUniformMatrix4fv(m_inner->g_mv, 1, GL_FALSE, mv.ptr());
	glUniformMatrix4fv(m_inner->g_perspective, 1, GL_FALSE, projectMatrix.ptr());

	m_layer->Render();

	glUseProgram(program);
	static double i = 0;
	glUniform1f(g_exploder, 20.0f * (sinf(i) +1));
	i += 0.01;
	glUniformMatrix4fv(g_world, 1, GL_FALSE, Matrixf::identity().ptr());
	Matrixf view = camera->getInverseMatrix();
	view.postMult(projectMatrix);
	glUniformMatrix4fv(g_vp, 1, GL_FALSE, view.ptr());
	
	m_layer->Render();

	glUseProgram(m_pSky->program);
	m_pSky->render();
}

void XGLModel::Explode::initUniform()
{
	g_world = glGetUniformLocation(program, "g_world");
	g_vp = glGetUniformLocation(program, "g_vp");
	g_exploder = glGetUniformLocation(program, "g_exploder");
	//getErrorInformation(GetLastError());
	if (g_world < 0
		|| g_vp < 0
		|| g_exploder < 0)
	{
		XGLERROR("get uniform failed");
	}
}
