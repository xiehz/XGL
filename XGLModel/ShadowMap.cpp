#include "stdafx.h"
#include "IXMesh.h"
#include "ShadowMap.h"

namespace XGLModel {

	REGISTER(ShadowMap)

}

XGLModel::ShadowMap::ShadowMap()
{
	m_pMesh = new IXMesh();
}


XGLModel::ShadowMap::~ShadowMap()
{
	delete m_pMesh;
	m_pMesh = nullptr;
}

void XGLModel::ShadowMap::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	m_pMesh->LoadMesh("E:/2018/opengl/Assimp/nanosuit/nanosuit.obj");
}

void XGLModel::ShadowMap::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	Matrix cameraMatrix = camera->getInverseMatrix();

	glUniformMatrix4fv(g_mv, 1, GL_FALSE, cameraMatrix.ptr());
	glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_sampler, 0);

	static float d = 0.5f;
	Vec3f epos(d, d, d);
	//d += 0.0005f;

	glUseProgram(0);
	glPointSize(16.0f);
	epos = epos * cameraMatrix;
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_POINTS);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(epos.x(), epos.y(), epos.z());
	glEnd();
	glPushMatrix();
	glLoadMatrixf(cameraMatrix.ptr());
	glBegin(GL_POINTS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex3f(0.8f, 0.8f, 0.8f);
	glEnd();
	ModelGL::drawAxis(1.0f);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectMatrix.ptr());

	glUseProgram(program);

	m_pMesh->Render();

}

void XGLModel::ShadowMap::initUniform()
{

	g_mv = glGetUniformLocation(program, "mv");
	g_perspective = glGetUniformLocation(program, "perspective");
	g_sampler = glGetUniformLocation(program, "sampler2d");


	if (g_mv < 0
		|| g_perspective< 0
		|| g_sampler< 0)
	{
		XGLERROR("get uniform failed");
	}

}
