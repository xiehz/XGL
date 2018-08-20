#include "stdafx.h"
#include "SkyCube.h"
#include "Explode.h"

namespace XGLModel {

	REGISTER(Explode)

}

XGLModel::Explode::Explode()
{
	m_layer = 0;
}


XGLModel::Explode::~Explode()
{
	if (m_layer)
		delete m_layer;
	m_layer = 0;
}

void XGLModel::Explode::initGL()
{
	m_layer = new IXMesh();
	m_layer->LoadMesh("E:/2018/opengl/Assimp/data/closed.obj");
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
}
void XGLModel::Explode::setExploder(float ex)
{
	glUniform1f(g_exploder, ex);
}

void XGLModel::Explode::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glUseProgram(program);

	glUniform1f(g_exploder, 20.0f);
	glUniformMatrix4fv(g_world, 1, GL_FALSE, Matrixf::identity().ptr());
	Matrixf view = camera->getInverseMatrix();
	view.postMult(projectMatrix);
	glUniformMatrix4fv(g_vp, 1, GL_FALSE, view.ptr());
	
	m_layer->Render();
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
